#ifndef CLIENTACTIONTONE_H
#define CLIENTACTIONTONE_H

#include <QTimer>
#include <fmod.h>
#include <fmod_errors.h>
#include "../clientaction.h"

class ClientActionTone : public ClientAction
{
    Q_OBJECT
public:
    ClientActionTone(AbstractServer *parent);
    ~ClientActionTone();

    virtual bool runAction();
    virtual bool setParamsFromMessage(QString message);

private slots:
    void update();
    void tonePeriod();

private:
    static unsigned int oct_freq[9][7];
    static float diez_k;
    static float bemol_k;

    bool prog_led_tone_control;
    bool tone_state, tone_periodic, tone_is_melody;
    unsigned long int tone_frequency, tone_period;

    FMOD_SYSTEM *system;
    FMOD_CHANNEL *channel;
    FMOD_DSP *dsp;
    QTimer *updateTimer;
    QTimer *periodicToneTimer;

    QString melody;
    int sub_level;
    unsigned int melody_pos;
    unsigned int melody_tempo;
    quint8 melody_timer_counter;
    quint8 melody_timer_counter_max;
    unsigned long timer_counter;
    unsigned long timer_counter_max;

    void ERRCHECK(FMOD_RESULT op_result);
    unsigned long readIntFromString(QString message, unsigned *pos);
    void tone();
    void notone();
    void setProgLed(bool state);
    void toneMelodyAction();
    void startMelodyTone(QString cmelody);
};

#endif // CLIENTACTIONTONE_H
