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

    QMap<QString, QString> *getSettings();
    virtual bool setParamsFromMessage(QString message);
    virtual QString getDescriptionString();
    virtual void setEnabled(bool enabled);
    bool isEnabled() const;
    QString getItemName() const;

signals:

protected:
    QMap<QString, QString> *settings;
    bool is_enabled;
    QString item_name;
};

#endif // CLIENTITEM_H
