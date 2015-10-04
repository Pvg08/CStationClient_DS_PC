#include "clientactionindication.h"

ClientActionIndication::ClientActionIndication(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "LED_SET";
    item_name = tr("Set indication");
    description_string = "{CODE:'led',PREFIX:'LED_SET',PARAM:{0:{NAME:'Led state',TYPE:'BOOL'}}}";
}

bool ClientActionIndication::runAction()
{
    if (is_enabled) emit updateState();
    return true;
}
