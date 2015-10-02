#ifndef CLIENTSENSORACTIVITY_H
#define CLIENTSENSORACTIVITY_H

#include "../clientsensor.h"

class ClientSensorActivity : public ClientSensor
{
    Q_OBJECT
public:
    ClientSensorActivity(AbstractServer *parent);
    virtual QString getValueString();
};

#endif // CLIENTSENSORACTIVITY_H
