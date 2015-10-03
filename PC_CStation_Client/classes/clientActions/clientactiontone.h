#ifndef CLIENTACTIONTONE_H
#define CLIENTACTIONTONE_H

#include <QTimer>
#include <fmod.hpp>
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
    bool prog_led_tone_control;
    bool tone_state, tone_periodic;
    unsigned long int tone_frequency, tone_period;

    FMOD::System *system;
    FMOD::Channel *channel;
    FMOD::DSP *dsp;
    QTimer *updateTimer;
    QTimer *periodicToneTimer;

    void ERRCHECK(FMOD_RESULT op_result);
    unsigned long readIntFromString(QString message, unsigned *pos);
    void tone();
    void notone();
    void setProgLed(bool state);
};

#endif // CLIENTACTIONTONE_H
