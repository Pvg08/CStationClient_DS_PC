#ifndef CLIENTSENSORACTIVITY_H
#define CLIENTSENSORACTIVITY_H

#include "../clientsensor.h"

class ClientSensorActivity : public ClientSensor
{
    Q_OBJECT
public:
    ClientSensorActivity(QObject *parent);
    virtual QString getDescriptionString();
    virtual QString getValueString();
};

#endif // CLIENTSENSORACTIVITY_H
