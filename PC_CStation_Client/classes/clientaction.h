#ifndef CLIENTACTION_H
#define CLIENTACTION_H

#include <QObject>
#include <QMap>
#include "clientitem.h"
#include "../abstractserver.h"

class ClientAction : public ClientItem
{
    Q_OBJECT
public:
    explicit ClientAction(AbstractServer *parent);

    virtual bool runAction();
    QString getPrefix() const;

signals:
    void updateState();

protected:
    QString param_prefix;
};

#endif // CLIENTACTION_H
