#ifndef CLIENTITEM_H
#define CLIENTITEM_H

#include <QObject>
#include <QMap>
#include "../abstractserver.h"

class ClientItem : public QObject
{
    Q_OBJECT
public:
    explicit ClientItem(AbstractServer *parent);
    ~ClientItem();

    ClientItemSettings *getSettings();
    virtual bool setParamsFromMessage(QString message);
    virtual QString getDescriptionString();
    virtual void setEnabled(bool enabled);
    bool isEnabled() const;
    QString getItemName() const;

signals:

protected:
    ClientItemSettings *settings;
    bool is_enabled;
    QString item_name;
    QString description_string;
};

#endif // CLIENTITEM_H
