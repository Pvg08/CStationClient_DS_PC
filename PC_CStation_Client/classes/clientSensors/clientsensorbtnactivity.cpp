#include "clientsensorbtnactivity.h"

ClientSensorBtnActivity::ClientSensorBtnActivity(AbstractServer *parent):
    ClientSensor(parent)
{
    letter = 'O';
}

QString ClientSensorBtnActivity::getDescriptionString()
{
    return QString(letter) + ":enum(no,yes)[]|Outer signal";
}

QString ClientSensorBtnActivity::getValueString()
{
    return QString(letter) + "("+settings->value("VALUE","no")+")";
}
