#include "clientactionindication.h"

ClientActionIndication::ClientActionIndication(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "LED_SET";
    item_name = tr("Set indication");
}

QString ClientActionIndication::getDescriptionString()
{
    return "Led";
}

bool ClientActionIndication::runAction()
{
    if (is_enabled) emit updateState();
    return true;
}
