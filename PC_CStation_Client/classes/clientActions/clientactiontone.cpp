#include "clientactiontone.h"

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
        result = FMOD_DSP_Release(dsp);
        ERRCHECK(result);
        result = FMOD_System_Close(system);
        ERRCHECK(result);
        result = FMOD_System_Release(system);
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
        result = FMOD_System_Create(&system);
        ERRCHECK(result);
        FMOD_System_SetDSPBufferSize(system, 64, 1);
        result = FMOD_System_Init(system, 2, FMOD_INIT_STREAM_FROM_UPDATE | FMOD_INIT_MIX_FROM_UPDATE, NULL);
        ERRCHECK(result);
        result = FMOD_System_CreateDSPByType(system, FMOD_DSP_TYPE_OSCILLATOR, &dsp);
        ERRCHECK(result);
    }

    if (channel) {
        result = FMOD_Channel_Stop(channel);
        ERRCHECK(result);
        channel = 0;
    }

    if (tone_frequency) {

        result = FMOD_DSP_SetParameterFloat(dsp, FMOD_DSP_OSCILLATOR_RATE, (float) tone_frequency);
        ERRCHECK(result);
        result = FMOD_System_PlayDSP(system, dsp, 0, true, &channel);
        ERRCHECK(result);
        result = FMOD_Channel_SetVolume(channel, 1.0f);
        ERRCHECK(result);

        result = FMOD_DSP_SetParameterInt(dsp,FMOD_DSP_OSCILLATOR_TYPE, 0);
        ERRCHECK(result);
        if (tone_periodic) {
            periodicToneTimer->setInterval(tone_period);
            periodicToneTimer->start();
        }
        tone();

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
        result = FMOD_Channel_SetPaused(channel,false);
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
        result = FMOD_Channel_SetPaused(channel,true);
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
        result = FMOD_System_Update(system);
        ERRCHECK(result);
    }
}

void ClientActionTone::tonePeriod()
{
    if (tone_state) {
        tone();
    } else {
        notone();
    }
    tone_state = !tone_state;
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
