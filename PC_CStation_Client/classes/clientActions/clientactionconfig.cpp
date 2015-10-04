#include "clientactionconfig.h"

ClientActionConfig::ClientActionConfig(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "SERV_CONF";
    item_name = tr("Configuration mode");
    description_string = "{CODE:'config',PREFIX:'SERV_CONF',PARAM:{0:{VALUE:1,SKIP:1}}}";
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
