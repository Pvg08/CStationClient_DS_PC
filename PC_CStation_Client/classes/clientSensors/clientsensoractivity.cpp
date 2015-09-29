#include "clientsensoractivity.h"

ClientSensorActivity::ClientSensorActivity(AbstractServer *parent):
    ClientSensor(parent)
{
    letter = 'A';
}

QString ClientSensorActivity::getDescriptionString()
{
    return QString(letter) + ":enum(off,on)[30]|Activity";
}

QString ClientSensorActivity::getValueString()
{
    return QString(letter) + "(on)";
}
