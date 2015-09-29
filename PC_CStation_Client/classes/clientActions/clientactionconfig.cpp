#include "clientactionconfig.h"

ClientActionConfig::ClientActionConfig(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "SERV_CONF";
}

QString ClientActionConfig::getDescriptionString()
{
    return "Config";
}

bool ClientActionConfig::runAction()
{
    bool result = false;
    AbstractServer *server = dynamic_cast<AbstractServer*>(this->parent());
    if (server != NULL) {
        server->ConfigurationMode();
        result = true;
    }
    return result;
}

