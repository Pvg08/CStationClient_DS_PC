#include "clientactiontonemelody.h"

unsigned int ClientActionToneMelody::oct_freq[9][7] = {
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
float ClientActionToneMelody::diez_k = 1.059463;
float ClientActionToneMelody::bemol_k = 0.9438743;

quint8 ClientActionToneMelody::melody_count = 17;

const QString melody_1 = "150:G4,E5,E5,D5,E5,C5,G4,G4,G4,E5,E5,F5,D5,G5,,G5,A4,A4,F5,F5,E5,D5,C5,G4,E5,E5,D5,E5,C5,,G5,A4,A4,F5,F5,E5,D5,C5,G4,E5,E5,D5,E5,C5";
const QString melody_2 = "220:B5b,F5,B5b,F5,B5b,A5,A5,,A5,F5,A5,F5,A5,B5b,B5b,,B5b,F5,B5b,F5,B5b,A5,A5,,A5,F5,A5,F5,A5,B5b,,,B5b,C6=6,p2,C6=3,p1,C6=3,p1,C6=6,p2,C6,C6#=6,p2,C6#=3,p1,C6#=3,p1,C6#=7,p1,C6#=7,p1,C6#,C6,B5b,A5,B5b,B5b,,,B5b,C6=6,p2,C6=3,p1,C6=3,p1,C6=6,p2,C6,C6#=6,p2,C6#=3,p1,C6#=3,p1,C6#=7,p1,C6#=7,p1,C6#,C6,B5b,A5,B5b";
const QString melody_3 = "140-2:B6b,F7,D7#,F7,G7#,F7,F7,,B6b,F7,D7#,F7,B7b,F7,F7,,,C8#,C8,B7b,G7#,B7,F7,F7,,,C8#,C8,B7b,G7#,C8,F7,F7,,,B6b,F7,D7#,F7,G7b,F7,F7,,,B6b,F7,D7#,F7,B7b,F7,F7,,,B7b,F7,F7";
const QString melody_4 = "160:F6#=3,p1,F6#=3,p1,F6#=3,p1,E6=3,p1,D6=3,p1,C6#=3,p1,B5,B5=3,p1,B5=3,p1,B5=3,p1,B5b=3,p1,F5#=3,p1,G5=3,p1,F5#=5,p3,F6#=3,p1,F6#=3,p1,F6#=3,p1,E6=3,p1,D6=3,p1,C6#=3,p1,B5=3,p5,B5=3,p1,B5=3,p1,B5=3,p1,B5b=3,p1,F5#=3,p1,G5=3,p1,F5#=5,p3,F6#=3,p1,F6#=3,p1,F6#=3,p1,E6=3,p1,D6=3,p1,C6#=3,p1,B5=3,p1,B5=5,p3,B5=3,p5,C6#=5,D6=3,p1,C6#=3,p1,B5=3,p8,p1,F6#=5,p3,E6=3,p1,D6=3,p1,C6#=3,p1,B5=3,p1,B5=5,p8,p3,C6#=3,p1,D6=3,p1,C6#=3,p1,B5=5";
const QString melody_5 = "200:D5#=6,p8,p8,p8,p2,F5=8,D5#=8,D5=8,p1,C5=4,D5#=5,p8,p8,p8,p7,F5=8,D5#=8,D5=8,p1,C5=4,F5=4,p8,p8,p8,p8,p1,G5=7,F5=8,D5#=8,p1,D5=5,F5=3,p8,p8,p8,p8,p1,G5=7,F5=8,D5#=8,p1,D5=4,D5#=3,p8,p8,p8,p8,F5=8,D5#=8,D5=8,p1,C5=4,D5#=3,p8,p8,p8,p8,p1,F5=7,D5#=8,D5=8,C5=4,D5=4,p8,p8,p8,p4,D5=4,D5#=8,p1,D5=8,C5=8,B4b=4,D5=4";
const QString melody_6 = "150:E5=4,F5,D5=4,E5,E5=4,F5,D5,E5,C5,B4,B4,D5,D5,,,p4,E5=4,F5,D5,E5=4,C5=4,D5,D5=4,B4,B4=4,A4=4,B4=4,C5,C5,p8,p8,p8,p4,E5=4,F5,D5=4,E5,E5=4,F5,F5=4,D5=4,E5,E5=4,C5=4,B4,B4,D5,D5=4,p8,p8,p4,A4=4,F5,F5=4,E5=4,E5,B4=4,D5,D5,C5=4,C5,B4=4,A4,A4,A4=4";
const QString melody_7 = "180:G4=4,F4=3,p1,F4,p8,p8,p8,A4=4,B4=4,C5#=4,D5=4,E5=3,p2,F5=3,p1,E5=6,p2,D5=2,p2,D5,D5,D5,D5=3,p1,D5=2,p2,D5=4,p1,C5=4,B4b=4,A4=4,G4=4,B4b=7,p1,A4=4,A4,A4=2,p7,G4=4,F4,A4=5,G4,G4=7,p1,D4=4,F4=7,p2,A4=3,p1,A4=6,p8,p6,G4=4,F4=3,p1,F4,p8,p8,p8,A4=4,B4=4,C5#=4,D5=4,E5=3,p2,F5=3,p1,E5=6,p2,D5=2,p2,D5,D5,D5,D5=3,p1,D5=2,p2,D5=4,p1,C5=4,B4b=4,A4=4,G4=4,B4b=7,p1,A4=4,A4,A4=2,p7,G4=4,F4,A4=5,G4,G4=7,p1,D4=4,F4=7,p2,A4=3,p1,A4=6";
const QString melody_8 = "160-1:E5=1,p3,A5,A5=2,p2,C6=1,p3,E6=2,p2,F6=1,p3,E6=2,p4,C6=1,p1,A5=4,,E6=2,p2,D6,D6=1,p3,C6=2,p2,B5=2,p2,A5=2,p2,E5=2,,,p2,E5=2,p2,A5,A5=1,p3,C6=1,p3,E6=2,p2,F6=1,p3,E6=2,p4,C6=2,A5=3,,p1,A5=2,p2,E6,E6=2,p2,D6=2,p2,C6=2,p2,B5=2,p2,A5,A5=4,p4,G5=6,p2,C6=5,p2,G5=5,p3,C6=2,p2,D6=2,p3,E6=3,p3,C6=1,p1,C6,C6=1,p3,E6=2,p2,G6,G6=3,F6=3,p2,E6=2,p2,D6=2,p2,E6,E6=3,p1,D6=3,p2,C6=3,p1,B5=2,p2,A5=2,p6,C6=2,p6,E6=2,p2,F6=2,p2,E6=3,p4,C6=1,A5=7,p5,A5=2,p2,E6,E6=2,p2,D6=2,p2,C6=4,B5=2,p2,A5,A5=6";
const QString melody_9 = "180:A5=3,p1,B5=1,p3,C6=4,A5=1,p3,B5=4,C6=2,p8,p8,p2,B5=4,A5=1,p3,B5=4,C6=2,p8,p8,p2,B5=4,A5=2,p2,C6=2,p2,C6=5,p3,A5,A5=1,p8,p8,p8,p3,A5=4,B5=1,p3,C6=3,p1,A5=1,p3,B5=4,C6=2,p8,p8,p2,B5=4,A5=1,p3,B5=4,C6=2,p8,p8,p2,B5=4,A5=1,p3,C6=1,p3,C6=7,p1,A5,A5=4,p8,p8,A5=4,F6,F6,F6=5,p3,G6=2,F6=2,D6=4,E6,E6,E6,E6=5,p8,p7,F6=3,p1,E6=1,p3,F6=3,p1,E6=1,p3,A5=4,D6,D6,p4,C6=5,p3,B5=5,p3,A5=3,p1,B5=2,p2,C6=3,p1,A5=2,p2,B5=4,C6=3,p8,p8,p1,B5=4,A5=3,p1,C6=1,p3,C6=7,p2,A5=5";
const QString melody_10 = "180:A5=7,p1,C6=6,p2,A5=2,p2,A5=2,p2,E5=7,p1,A5=8,C6=6,p2,A5=2,p2,A5=2,p2,F5=6,p2,D5=6,p2,F5=6,p2,E5=2,p2,E5=2,p2,C5=7,p1,E5=2,p2,D5=2,p2,C5=2,p2,D5=2,p2,E5=8,E5=5,p3,A5=6,p2,C6=7,p1,A5=2,p2,A5=2,p2,E5=6,p2,A5=7,p1,C6=6,p2,A5=2,p2,A5=1,p3,F5=6,p2,D5=6,p2,F5=6,p2,E5=2,p2,E5=2,p2,C5=6,p2,E5=4,D5=1,p3,C5=1,p3,B4=2,p2,A4=7";
const QString melody_11 = "145:G5#=4,C6#=4,E6=4,A6=6,G6#=2,G6#=8,p2,G6#=2,F6#=2,E6=2,D6#=4,C6#=4,A6=8,G6#=8,G6#=2,p2,G6#=4,G6=4,G6#=4,A6=6,G6#=2,G6#=8,p2,G6#=2,G6=2,G6#=2,C7#=4,A6=4,G6#=7,p1,F6#=8,,F6#=2,F6#=4,E6=4,D6#=4,C7#=8,C7#=4,B6=4,A6=4,G6#=8,E6=4,D6#=4,C6#=4,B6=4,A6=4,G6#=4,F6#=8,C6#=4,C6=4,C6=4,C6=4,C6=4,C6=4,C6#=2,C6=2,B5b=4,C6=4,C6#=8,C6#=8,C6#=4";
const QString melody_12 = "160:E6=3,p1,D6=3,p1,C6=3,p1,D6=8,D6=3,p1,E6=2,p2,F6=5,p1,E6=2,D6=3,p1,B5=2,C6=8,C6=7,p7,E6=3,p1,D6=3,p1,C6=2,p2,D6=8,D6=3,C6=3,p2,B5=3,C6=2,p1,B5=5,p1,E5=2,C6=8,C6=6,p8,G6=3,p1,F6=3,p1,E6=3,p1,D6=8,D6=4,p6,F6=1,p1,F6=1,p1,F6=1,p1,E6=3,p1,D6=2,p2,E6=8,E6=5,p8,p8,p2,E5=1,p1,E6=2,D6=2,p1,D6=3,p1,C6=5,p1,B5=3,p1,C6=2,p1,B5=5,p1,A5=2,p1,C6=8,C6=5";
const QString melody_13 = "160:C6=1,p3,C6=1,p3,C6=1,p3,C6=4,p4,B5=4,p4,D6=8,C6=4,G5#=5,p8,p8,p3,C6=1,p3,C6=1,p3,C6=4,D6#=8,D6=5,p3,D6=8,C6=3,p1,G5=5,p8,p8,p3,G5=1,p3,G5=1,p3,G5=1,p3,G5#=5,p3,G5=5,p3,D6=8,D6=4,B5=4,G5=4,p4,G5=8,F6=6,p2,F6=7,p1,D6#=8,D6=8,C6=2,p2,C6=1,p3,C6=2,p2,C6=1,p3,C6=5,p3,B5=5,p3,D6=8,C6=5,G5#=5,p8,p8,p3,C6=1,p3,C6=1,p3,C6=1,p3,D6#=5,p3,D6=5,p3,D6=8,C6=4,G5=5,p8,p8,p3,G5=1,p3,G5=1,p3,G5=1,p3,G5#=5,p3,G5=5,p3,D6=8,D6=4,B5=4,G5=4,p4,G5=8,F6=5,p3,F6=8,D6#=8,D6=8,C6=4";
const QString melody_14 = "160:F6#=2,p2,F6#=2,p2,F6#=2,p2,F6#=4,p1,B6=4,p3,B6=5,p2,B6=2,p3,B6=2,p2,B6=2,p2,B6=4,p1,B6b=3,p8,p4,F6#=2,p2,F6#=2,p2,F6#=2,p2,F6#=5,p1,C7#=4,p2,C7#=6,p2,C7#=3,p1,D7=3,p1,C7#=3,p2,B6=5,p8,p7,F6#=2,p1,F6#=2,p2,F6#=2,p2,F6#=4,p2,B6=4,p3,B6=6,p2,B6=2,p2,B6=2,p3,B6=2,p2,B6=5,B6b=3,p8,p4,F6#=2,p2,F6#=2,p2,F6#=2,p2,F6#=5,p1,C7#=3,p3,C7#=4,p4,C7#=4,D7=4,p1,C7#=3,p1,B6=5";
const QString melody_15 = "260:C4=3,C4=4,p1,A3=2,C4=3,p5,C4=3,p8,p2,C4=3,C4=5,A3=2,C4#=3,p4,C4#=3,p8,p3,C4#=3,C4#=4,p1,A3=2,p1,D4=3,p5,D4=3,p4,C4=3,p5,B3b=4,p1,A3=7,p8,p8,p1,A3=3,p1,B3b=4,p1,C4=3,D4=4,p4,D4=5,p8,D3=3,E3=5,F3=4,A3=4,p4,A3=4,p4,G3=4,p1,A3=5,p4,C4=7,p8,p4,E4=4,p1,E4=5,p3,C4=5,p8,p8,p2,C4=2,C4=4,p1,A3=3,C4=4,p4,C4=3,p8,p2,C4=2,p1,C4=4,p1,A3=3,C4#=4,p5,C4#=3,p8,p2,C4#=2,p1,C4#=4,A3=2,D4=3,p5,D4=3,p4,C4=3,p5,B3b=4,p1,A3=8,p8,p8,A3=3,B3b=5,C4=3,D4=4,p4,D4=4,p4,D3=6,E3=5,F3=4,p1,A3=5,p3,A3=4,p4,A3=6,G3#=3,G3=5,F3=7";
const QString melody_16 = "200-2:A6=6,p2,A6=2,p2,A6=2,p2,G6#=6,p2,A6=6,p2,C7=6,p2,B6b=8,B6b=4,p4,A6=6,p2,G6=6,p2,G6=2,p2,G6=2,p2,B6b=6,p2,A6=6,p2,G6=6,p2,F6=8,F6=2,p6,E6=6,p2,D6=6,p2,D6=4,p4,D6=8,C6=4,B5b=4,D6=6,p2,D6=6,p2,D6=8,E6=4,F6=4,G6=8,G6=8,G6=8,G6=8,p8,F6=8,E6=8,D6=4,C6=4,F6=6,p2,F6=6";
const QString melody_17 = "180-1:G6=4,C7=5,B6=2,C7=6,B6=2,A6=8,p4,G6=4,A6=6,G6#=2,A6=6,E6=2,F6=8,p4,G6=4,B6=6,B6b=2,B6=6,A6=2,G6=8,p4,G6#=4,A6=6,G6#=2,A6=6,F6=2,E6=8,p6,G6=2,C7=6,B6=2,C7=6,B6=2,A6=8,p6,G6=2,A6=2,p2,G6=2,p2,F6=2,p2,E6=2,p2,D6=8,p6,A6=1,p1,A6=2,p2,A6=2,p2,B6=2,p2,A6=3,p1,G6=6,p2,A6=2,p2,G6=3,p1,F6=6,p2,B5=6,p2,G6=8,p6,A6=1,p1,A6=2,p2,A6=2,p2,B6=2,p2,A6=3,p1,G6=6,p2,B6=2,p2,C7=2,p2,B6=6,p2,G6=6,p2,C7=8";

const QString ClientActionToneMelody::melody_list[] = {melody_1, melody_2, melody_3, melody_4, melody_5, melody_6, melody_7, melody_8, melody_9, melody_10, melody_11, melody_12, melody_13, melody_14, melody_15, melody_16, melody_17};

ClientActionToneMelody::ClientActionToneMelody(AbstractServer *parent):
    ClientActionTone(parent)
{
    param_prefix = "MEL";
    item_name = tr("Melody");
    description_string = "{'CODE':'melody','PREFIX':'MEL','PARAM':[{'NAME':'Write to buffer','SKIP':1,'VALUE':'B','TYPE':'BOOL'},{'NAME':'Code as index','SKIP':1,'VALUE':'I','TYPE':'BOOL'},{'NAME':'Code','TYPE':'STRING'}],'BUTTONS':[{'NAME':'Reset','PARAMSET':['0']}]}";

    melody_0 = "160:C6=1,G6=4,A6=6";
    melody = "";
}

bool ClientActionToneMelody::runAction()
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

        result = FMOD_DSP_SetParameterInt(dsp,FMOD_DSP_OSCILLATOR_TYPE, 1);
        ERRCHECK(result);
        StartMelodyTone(settings->value("MELODY"));

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

void ClientActionToneMelody::tonePeriod()
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

void ClientActionToneMelody::toneMelodyAction()
{
    FMOD_RESULT result;

    if (melody_timer_counter<melody_timer_counter_max) {
        melody_timer_counter++;
        return;
    }
    melody_timer_counter = 0;
    if (melody_pos>=melody.length()) {
        result = FMOD_DSP_SetParameterFloat(dsp, FMOD_DSP_OSCILLATOR_RATE, 0.001);
        ERRCHECK(result);
        result = FMOD_Channel_SetVolume(channel, 0.0f);
        ERRCHECK(result);
        periodicToneTimer->stop();
        return;
    }
    if (melody[melody_pos]==',') melody_pos++;
    if (melody[melody_pos]==',' || melody_pos>=melody.length()) {
        result = FMOD_DSP_SetParameterFloat(dsp, FMOD_DSP_OSCILLATOR_RATE, 0.001);
        ERRCHECK(result);
        result = FMOD_Channel_SetVolume(channel, 0.0f);
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
            result = FMOD_DSP_SetParameterFloat(dsp, FMOD_DSP_OSCILLATOR_RATE, (float) cfreq);
            ERRCHECK(result);
            tone();
            result = FMOD_Channel_SetVolume(channel, 1.0f);
            ERRCHECK(result);
        } else {
            result = FMOD_DSP_SetParameterFloat(dsp, FMOD_DSP_OSCILLATOR_RATE, 0.001);
            ERRCHECK(result);
            result = FMOD_Channel_SetVolume(channel, 0.0f);
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

void ClientActionToneMelody::StartMelodyTone(QString cmelody)
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

void ClientActionToneMelody::StartMelodyToneByIndex(quint8 index) {
  if (index == 0) {
    StartMelodyTone(melody_0);
  } else if (index <= melody_count) {
    StartMelodyTone(melody_list[index-1]);
  }
}

bool ClientActionToneMelody::setParamsFromMessage(QString message)
{
    prog_led_tone_control = false;
    tone_state = false;
    tone_periodic = false;
    tone_frequency = 1;
    tone_period = 0;

    bool to_buf = false;
    bool by_index = false;
    if (message[0] == 'B') {
        to_buf = true;
        message.remove(0,1);
        if (message[0] == ',') message.remove(0,1);
    }
    if (message[0] == 'I') {
        by_index = true;
        message.remove(0,1);
        if (message[0] == ',') message.remove(0,1);
    }
    if (message[0] == 'B') {
        to_buf = true;
        message.remove(0,1);
        if (message[0] == ',') message.remove(0,1);
    }

    if (by_index) {
        unsigned index = readIntFromString(message, 0);
        StartMelodyToneByIndex(index);
    } else {
        settings->insert("MELODY",message);
        if (to_buf) {
            melody_0 = message;
        }
        StartMelodyTone(message);
    }

    return true;
}
