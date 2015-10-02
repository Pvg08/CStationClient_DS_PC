#include "clientitem.h"

ClientItem::ClientItem(AbstractServer *parent) : QObject(parent)
{
    settings = new ClientItemSettings();
    is_enabled = true;
    item_name = tr("Item");
    description_string = "";
}

ClientItem::~ClientItem()
{
    delete settings;
}

ClientItemSettings *ClientItem::getSettings()
{
    return settings;
}

bool ClientItem::setParamsFromMessage(QString message)
{
    settings->insert("VALUE", message);
    return true;
}

QString ClientItem::getDescriptionString()
{
    return description_string;
}

void ClientItem::setEnabled(bool enabled)
{
    is_enabled = enabled;
}

bool ClientItem::isEnabled() const
{
    return is_enabled;
}

QString ClientItem::getItemName() const
{
    return item_name;
}
