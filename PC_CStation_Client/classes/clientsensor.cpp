#include "clientsensor.h"

ClientSensor::ClientSensor(AbstractServer *parent) : QObject(parent)
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

void ClientSensor::sendNow()
{
    if (is_enabled) {
        AbstractServer *server = dynamic_cast<AbstractServer*>(this->parent());
        if (server != NULL) {
            emit sendingInitiate(getValueString());
        }
    }
}

bool ClientSensor::isEnabled() const
{
    return is_enabled;
}
