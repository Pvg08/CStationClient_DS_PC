#include "clientactionlcd.h"

ClientActionLcd::ClientActionLcd(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "SERV_LT";
    item_name = tr("Display text");
    description_string = "LCD";
    fixed_mode = false;
    display_text = "";

    AbstractServer *server = dynamic_cast<AbstractServer*>(parent);
    if (server) {
        connect(server, SIGNAL(beforeSensorsSending()), this, SLOT(sensorsReady()));
    }
}

bool ClientActionLcd::runAction()
{
    if (!is_enabled) return false;
    if (fixed_mode) {
        emit updateState();
    } else {
        sensorsReady();
    }
    return true;
}

bool ClientActionLcd::setParamsFromMessage(QString message)
{
    QString trmessage = message.trimmed();
    if (trmessage.isEmpty()) {
        message = "";
    }
    fixed_mode = !message.isEmpty();
    settings->insert("FIXED", fixed_mode ? "1" : "0");
    settings->insert("TEXT", fixed_mode ? message : display_text);
    return true;
}

void ClientActionLcd::sensorsReady()
{
    if (!fixed_mode && is_enabled) {
        display_text = "";
        AbstractServer *server = dynamic_cast<AbstractServer*>(this->parent());
        if (server) {
            ClientItemSettings* btn_settings = server->GetItemSettings("button_activity");
            if (btn_settings) {
                display_text = "Button: ";
                if (btn_settings->value("VALUE", "no") == "yes") {
                    display_text += "Pressed";
                } else {
                    display_text += "Released";
                }
            }
        }
        settings->insert("TEXT", display_text);
        emit updateState();
    }
}
