#include "clientitem.h"

ClientItem::ClientItem(AbstractServer *parent) : QObject(parent)
{
    settings = new QMap<QString, QString>();
    is_enabled = true;
    item_name = tr("Item");
}

ClientItem::~ClientItem()
{
    delete settings;
}

QMap<QString, QString> *ClientItem::getSettings()
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
    return "";
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
