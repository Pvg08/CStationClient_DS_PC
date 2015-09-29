#include "clientsensor.h"

ClientSensor::ClientSensor(QObject *parent) : QObject(parent)
{
    settings = new QMap<QString,QString>();
    letter = '0';
    is_enabled= true;
}

ClientSensor::~ClientSensor()
{
    delete settings;
}

QMap<QString, QString> *ClientSensor::getSettings()
{
    return settings;
}

QString ClientSensor::getDescriptionString()
{
    return "";
}

QString ClientSensor::getValueString()
{
    return "";
}

void ClientSensor::setEnabled(bool enabled)
{
    is_enabled = enabled;
}
