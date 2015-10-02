#include "clientactionconfig.h"

ClientActionConfig::ClientActionConfig(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "SERV_CONF";
    item_name = tr("Configuration mode");
    description_string = "Config";
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

