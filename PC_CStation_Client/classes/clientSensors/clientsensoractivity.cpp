#include "clientsensoractivity.h"

ClientSensorActivity::ClientSensorActivity(AbstractServer *parent):
    ClientSensor(parent)
{
    letter = 'A';
    item_name = tr("Client activity");
    description_string = QString(letter) + ":enum(off,on)[60]|Activity";
}

QString ClientSensorActivity::getValueString()
{
    return QString(letter) + "(on)";
}
