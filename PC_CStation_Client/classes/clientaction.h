#ifndef CLIENTACTION_H
#define CLIENTACTION_H

#include <QObject>
#include <QMap>
#include "../abstractserver.h"

class ClientAction : public QObject
{
    Q_OBJECT
public:
    explicit ClientAction(AbstractServer *parent);
    ~ClientAction();

    QMap<QString, QString> *getSettings();
    virtual bool setParamsFromMessage(QString message);
    virtual QString getDescriptionString();
    virtual bool runAction();
    virtual void setEnabled(bool enabled);
    bool isEnabled() const;
    QString getPrefix() const;

signals:

protected:
    QMap<QString, QString> *settings;
    QString param_prefix;
    bool is_enabled;
};

#endif // CLIENTACTION_H
