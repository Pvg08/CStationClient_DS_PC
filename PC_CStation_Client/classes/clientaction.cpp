#include "clientaction.h"

ClientAction::ClientAction(AbstractServer *parent) : ClientItem(parent)
{
    param_prefix = "";
}

bool ClientAction::runAction()
{
    return false;
}

QString ClientAction::getPrefix() const
{
    return param_prefix;
}
