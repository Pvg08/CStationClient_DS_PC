#include "clientsensorbtnactivity.h"

ClientSensorBtnActivity::ClientSensorBtnActivity(AbstractServer *parent):
    ClientSensor(parent)
{
    letter = 'O';
    item_name = tr("Action button");
    description_string = QString(letter) + ":enum(no,yes)[]|Outer signal";
}

QString ClientSensorBtnActivity::getValueString()
{
    return QString(letter) + "("+settings->value("VALUE","no")+")";
}
