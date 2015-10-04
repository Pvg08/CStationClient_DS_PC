#include "clientactionindicationstate.h"

ClientActionIndicationState::ClientActionIndicationState(AbstractServer *parent):
    ClientAction(parent)
{
    param_prefix = "STATES_REQUEST";
    item_name = tr("Indication state request");
    description_string = "{CODE:'state',PREFIX:'STATES_REQUEST',PARAM:{0:{VALUE:1,SKIP:1,LISTEN:1}}}";
}

bool ClientActionIndicationState::runAction()
{
    bool result = false;
    AbstractServer *server = dynamic_cast<AbstractServer*>(this->parent());
    if (server != NULL) {
        ClientItemSettings* led_settings = server->GetItemSettings("led");
        ClientItemSettings* tone_settings = server->GetItemSettings("tone");
        bool prog_led_state = led_settings && led_settings->value("VALUE", "0").toInt()!=0;
        bool tone_state = tone_settings && tone_settings->value("ACTIVE", "0").toInt()!=0;

        QString states_str = "DS_STATE={";
        states_str = states_str + "\"LED\":\""+(prog_led_state ? "on" : "off")+"\", ";
        states_str = states_str + "\"TONE\":\""+(tone_state ? "on" : "off")+"\"";
        states_str += "}\r\n";
        server->SendData(states_str);
        result = true;
    }
    return result;
}
