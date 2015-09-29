#include "clientaction.h"

ClientAction::ClientAction(AbstractServer *parent) : QObject(parent)
{
    settings = new QMap<QString, QString>();
    is_enabled = true;
}

ClientAction::~ClientAction()
{
    delete settings;
}

QMap<QString, QString> *ClientAction::getSettings()
{
    return settings;
}

bool ClientAction::setParamsFromMessage(QString message)
{
    settings->insert("VALUE", message);
    return true;
}

QString ClientAction::getDescriptionString()
{
    return "";
}

bool ClientAction::runAction()
{
    return false;
}

void ClientAction::setEnabled(bool enabled)
{
    is_enabled = enabled;
}

bool ClientAction::isEnabled() const
{
    return is_enabled;
}

QString ClientAction::getPrefix() const
{
    return param_prefix;
}


