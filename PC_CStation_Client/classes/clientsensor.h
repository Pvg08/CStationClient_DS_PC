#ifndef CLIENTSENSOR_H
#define CLIENTSENSOR_H

#include <QObject>
#include <QMap>
#include "clientitem.h"
#include "../abstractserver.h"

class ClientSensor : public ClientItem
{
    Q_OBJECT
public:
    explicit ClientSensor(AbstractServer *parent);

    virtual QString getValueString();
    virtual void sendNow();

signals:
    void sendingInitiate(QString);

protected:
    QChar letter;
};

#endif // CLIENTSENSOR_H
