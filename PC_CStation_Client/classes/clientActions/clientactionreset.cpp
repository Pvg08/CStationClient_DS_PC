#include "clientactionreset.h"

ClientActionReset::ClientActionReset(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "SERV_RST";
    item_name = tr("Reset");
    description_string = "Reset";
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

