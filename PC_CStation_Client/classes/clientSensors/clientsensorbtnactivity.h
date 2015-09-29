#ifndef CLIENTSENSORBTNACTIVITY_H
#define CLIENTSENSORBTNACTIVITY_H

#include "../clientsensor.h"

class ClientSensorBtnActivity : public ClientSensor
{
    Q_OBJECT
public:
    ClientSensorBtnActivity(AbstractServer *parent);
    virtual QString getDescriptionString();
    virtual QString getValueString();
};

#endif // CLIENTSENSORBTNACTIVITY_H
