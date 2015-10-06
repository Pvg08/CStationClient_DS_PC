#include "clientactionreset.h"

ClientActionReset::ClientActionReset(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "SERV_RST";
    item_name = tr("Reset");
    description_string = "{'CODE':'reset','PREFIX':'SERV_RST','PARAM':[{'VALUE':1,'SKIP':1}]}";
}

bool ClientActionReset::runAction()
{
    bool result = false;
    AbstractServer *server = dynamic_cast<AbstractServer*>(this->parent());
    if (server != NULL) {
        server->Reset();
        result = true;
    }
    return result;
}

