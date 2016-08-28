#ifndef CLIENTACTIONTONEMELODY_H
#define CLIENTACTIONTONEMELODY_H

#include <QTimer>
#include <fmod.h>
#include <fmod_errors.h>
#include "./clientactiontone.h"

#define MELODY_MAX_SIZE 512

class ClientActionToneMelody : public ClientActionTone
{
    Q_OBJECT
public:
    ClientActionToneMelody(AbstractServer *parent);

    virtual bool runAction();
    virtual bool setParamsFromMessage(QString message);

private slots:
    virtual void tonePeriod();

private:
    static unsigned int oct_freq[9][7];
    static float diez_k;
    static float bemol_k;
    static quint8 melody_count;
    static const QString melody_list[];
    QString melody_0;

    bool tone_is_melody;

    QString melody;
    int sub_level;
    unsigned int melody_pos;
    unsigned int melody_tempo;
    quint8 melody_timer_counter;
    quint8 melody_timer_counter_max;
    unsigned long timer_counter;
    unsigned long timer_counter_max;

    void toneMelodyAction();
    void StartMelodyTone(QString cmelody);
    void StartMelodyToneByIndex(quint8 index);
};

#endif // CLIENTACTIONTONE_H
