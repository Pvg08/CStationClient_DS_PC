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

protected slots:
    void update();
    virtual void tonePeriod();

protected:
    bool prog_led_tone_control;
    bool tone_state, tone_periodic;
    unsigned long int tone_frequency, tone_period;

    FMOD_SYSTEM *system;
    FMOD_CHANNEL *channel;
    FMOD_DSP *dsp;
    QTimer *updateTimer;
    QTimer *periodicToneTimer;

    void ERRCHECK(FMOD_RESULT op_result);
    unsigned long readIntFromString(QString message, unsigned *pos);
    void tone();
    void notone();
    void setProgLed(bool state);
};

#endif // CLIENTACTIONTONE_H
