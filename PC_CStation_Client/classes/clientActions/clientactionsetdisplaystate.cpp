#include "clientactionsetdisplaystate.h"

ClientActionSetDisplayState::ClientActionSetDisplayState(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "SET_DISPLAY_ST";
    item_name = tr("Set display state");
    description_string = "{CODE:'displaystate',PREFIX:'SET_DISPLAY_ST',PARAM:{0:{NAME:'Set display state',TYPE:'BOOL'}}}";
}

bool ClientActionSetDisplayState::runAction()
{
    SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) (settings->value("VALUE", "0").toInt() ? -1 : 2));
    return true;
}
