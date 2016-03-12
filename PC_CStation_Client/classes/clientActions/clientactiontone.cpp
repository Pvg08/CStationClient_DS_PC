#include "clientactiontone.h"

unsigned int ClientActionTone::oct_freq[9][7] = {
  {16,  18,  21,  22,  25,  28,  31  },
  {33,  37,  41,  44,  49,  55,  62  },
  {65,  73,  82,  87,  98,  110, 123 },
  {131, 147, 165, 175, 196, 220, 247 },
  {262, 294, 330, 349, 392, 440, 494 },
  {523, 587, 659, 698, 784, 880, 988 },
  {1046,1175,1319,1568,1760,1976,1967},
  {2093,2349,2637,2794,3136,3520,3951},
  {4186,4699,5274,5588,6272,7040,7902}
};
float ClientActionTone::diez_k = 1.059463;
float ClientActionTone::bemol_k = 0.9438743;

ClientActionTone::ClientActionTone(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "TONE";
    item_name = tr("Tone");
    description_string = "{'CODE':'tone','PREFIX':'TONE','PARAM':[{'NAME':'Led indication','SKIP':1,'VALUE':'L','TYPE':'BOOL'},{'NAME':'Frequency','TYPE':'UINT','DEFAULT':500},{'NAME':'Period','TYPE':'UINT'}],'BUTTONS':[{'NAME':'Reset','PARAMSET':['0']}]}";

    system = NULL;
    updateTimer = new QTimer();
    periodicToneTimer = new QTimer();

    updateTimer->setInterval(200);
    updateTimer->setSingleShot(false);

    periodicToneTimer->setInterval(1000);
    periodicToneTimer->setSingleShot(false);
    periodicToneTimer->setTimerType(Qt::PreciseTimer);

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    connect(periodicToneTimer, SIGNAL(timeout()), this, SLOT(tonePeriod()));
}

ClientActionTone::~ClientActionTone()
{
    delete updateTimer;
    delete periodicToneTimer;
    if (system) {
        FMOD_RESULT result;
        result = dsp->release();
        ERRCHECK(result);
        result = system->close();
        ERRCHECK(result);
        result = system->release();
        ERRCHECK(result);
    }
}

void ClientActionTone::ERRCHECK(FMOD_RESULT op_result)
{
    if (op_result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", op_result, FMOD_ErrorString(op_result));
        exit(-1);
    }
}

bool ClientActionTone::runAction()
{
    FMOD_RESULT result;
    if (!system) {
        channel = 0;
        result = FMOD::System_Create(&system);
        ERRCHECK(result);
        system->setDSPBufferSize(64, 1);
        result = system->init(2, FMOD_INIT_STREAM_FROM_UPDATE | FMOD_INIT_MIX_FROM_UPDATE, NULL);
        ERRCHECK(result);
        result = system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &dsp);
        ERRCHECK(result);
    }

    if (channel) {
        result = channel->stop();
        ERRCHECK(result);
        channel = 0;
    }

    if (tone_frequency) {

        result = dsp->setParameterFloat(FMOD_DSP_OSCILLATOR_RATE, (float) tone_frequency);
        ERRCHECK(result);
        result = system->playDSP(dsp, 0, true, &channel);
        ERRCHECK(result);
        result = channel->setVolume(1.0f);
        ERRCHECK(result);

        if (settings->value("MELODY").isEmpty()) {
            result = dsp->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, 0);
            ERRCHECK(result);
            if (tone_periodic) {
                periodicToneTimer->setInterval(tone_period);
                periodicToneTimer->start();
            }
            tone();
        } else {
            result = dsp->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, 1);
            ERRCHECK(result);
            startMelodyTone(settings->value("MELODY"));
        }

        updateTimer->start();
        settings->insert("ACTIVE", "1");
    } else {
        updateTimer->stop();
        periodicToneTimer->stop();
        prog_led_tone_control = false;
        tone_state = false;
        tone_periodic = false;
        tone_period = 0;
        settings->insert("ACTIVE", "0");
    }

    return true;
}

void ClientActionTone::setProgLed(bool state)
{
    AbstractServer *server = dynamic_cast<AbstractServer*>(this->parent());
    if (server) {
        ClientItemSettings* led_settings = server->GetItemSettings("led");
        if (led_settings) {
            led_settings->insert("VALUE", state ? "1" : "0");
            server->RunAction("led");
        }
    }
}

void ClientActionTone::tone()
{
    if (system) {
        FMOD_RESULT result;
        result = channel->setPaused(false);
        ERRCHECK(result);
        if (prog_led_tone_control) {
            setProgLed(true);
        }
    }
}

void ClientActionTone::notone()
{
    if (system) {
        FMOD_RESULT result;
        result = channel->setPaused(true);
        ERRCHECK(result);
        if (prog_led_tone_control) {
            setProgLed(false);
        }
    }
}

void ClientActionTone::update()
{
    if (system) {
        FMOD_RESULT result;
        result = system->update();
        ERRCHECK(result);
    }
}

void ClientActionTone::tonePeriod()
{
    if (tone_is_melody) {
        toneMelodyAction();
    } else {
        if (tone_state) {
            tone();
        } else {
            notone();
        }
        tone_state = !tone_state;
    }
}

void ClientActionTone::toneMelodyAction()
{
    FMOD_RESULT result;

    if (melody_timer_counter<melody_timer_counter_max) {
        melody_timer_counter++;
        return;
    }
    melody_timer_counter = 0;
    if (melody_pos>=melody.length()) {
        result = dsp->setParameterFloat(FMOD_DSP_OSCILLATOR_RATE, 0.001);
        ERRCHECK(result);
        result = channel->setVolume(0.0f);
        ERRCHECK(result);
        periodicToneTimer->stop();
        return;
    }
    if (melody[melody_pos]==',') melody_pos++;
    if (melody[melody_pos]==',' || melody_pos>=melody.length()) {
        result = dsp->setParameterFloat(FMOD_DSP_OSCILLATOR_RATE, 0.001);
        ERRCHECK(result);
        result = channel->setVolume(0.0f);
        ERRCHECK(result);
        if (melody_pos>=melody.length()) {
            periodicToneTimer->stop();
            return;
        }
    } else {
        char bukv = melody[melody_pos].toLatin1();
        if (bukv!='p') {
            melody_timer_counter_max = 8;
            quint8 b_pos = 0;
            switch(bukv) {
                case 'C': b_pos = 0; break;
                case 'D': b_pos = 1; break;
                case 'E': b_pos = 2; break;
                case 'F': b_pos = 3; break;
                case 'G': b_pos = 4; break;
                case 'A': b_pos = 5; break;
                case 'B': b_pos = 6; break;
            }
            melody_pos++;
            char cifr = melody[melody_pos].toLatin1();
            quint8 c_pos = cifr - '0';
            if (c_pos>=sub_level) c_pos-=sub_level; else c_pos=0;
            unsigned int cfreq = oct_freq[c_pos][b_pos];
            if (melody[melody_pos+1]=='#') {
                melody_pos++;
                cfreq = cfreq * diez_k;
            }
            if (melody[melody_pos+1]=='b') {
                melody_pos++;
                cfreq = cfreq * bemol_k;
            }
            if (melody[melody_pos+1]=='=') {
                melody_pos+=2;
                if (melody_pos>=melody.length()) {
                    return;
                }
                char pcifr = melody[melody_pos].toLatin1();
                melody_timer_counter_max = pcifr - '0';
                if (melody_timer_counter_max>9) melody_timer_counter_max = 9;
            }
            result = dsp->setParameterFloat(FMOD_DSP_OSCILLATOR_RATE, (float) cfreq);
            ERRCHECK(result);
            tone();
            result = channel->setVolume(1.0f);
            ERRCHECK(result);
        } else {
            result = dsp->setParameterFloat(FMOD_DSP_OSCILLATOR_RATE, 0.001);
            ERRCHECK(result);
            result = channel->setVolume(0.0f);
            ERRCHECK(result);
            //notone();
            melody_pos++;
            if (melody_pos>=melody.length()) {
                periodicToneTimer->stop();
            }
            char pcifr = melody[melody_pos].toLatin1();
            melody_timer_counter_max = pcifr - '0';
            if (melody_timer_counter_max>9) melody_timer_counter_max = 9;
        }
    }
    melody_pos++;
}

void ClientActionTone::startMelodyTone(QString cmelody)
{
    melody = cmelody;
    sub_level = 0;
    unsigned int pos = 0;
    unsigned int melody_ctemp = readIntFromString(melody, &pos);
    melody_timer_counter = 1;
    melody_timer_counter_max = 1;
    if (melody_ctemp) {
        melody_tempo = 7500 / melody_ctemp;
    } else {
        melody_tempo = 75;
    }
    if (melody[pos]=='-') {
        pos++;
        sub_level = readIntFromString(melody, &pos);
    }
    if (melody[pos] == ':') {
        pos++;
    }
    tone_frequency = 1;
    melody_pos = pos;
    tone_periodic = true;
    tone_state = true;
    tone_is_melody = true;
    prog_led_tone_control = false;
    toneMelodyAction();
    updateTimer->setInterval(melody_tempo / 2);
    periodicToneTimer->setInterval(melody_tempo);
    periodicToneTimer->start();
}

bool ClientActionTone::setParamsFromMessage(QString message)
{
    prog_led_tone_control = false;
    tone_state = false;
    tone_periodic = false;

    if (message[0] == 'L') {
        prog_led_tone_control = true;
        message.remove(0,1);
        if (message[0] == ',') message.remove(0,1);
    } else if (message[0] == 'M') {
        message.remove(0,1);
        if (message[0] == ':') message.remove(0,1);
        settings->insert("MELODY",message);
        tone_frequency = 1;
        tone_period = 0;
        return true;
    }
    unsigned fr_pos = 0;
    tone_frequency =  readIntFromString(message, &fr_pos);

    if (tone_frequency) {
        if (message[fr_pos] == ',') fr_pos++;
        tone_period = readIntFromString(message, &fr_pos);
        if (tone_period) {
            tone_periodic = true;
        }
    } else {
        tone_period = 0;
    }

    return true;
}

unsigned long int ClientActionTone::readIntFromString(QString message, unsigned *pos)
{
  unsigned long int result = 0;

  while (!message.isEmpty() && message[*pos]>='0' && message[*pos]<='9') {
    result = 10*result + QString(message[*pos]).toInt();
    (*pos)++;
  }

  return result;
}
