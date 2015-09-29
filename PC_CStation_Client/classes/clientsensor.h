#ifndef CLIENTSENSOR_H
#define CLIENTSENSOR_H

#include <QObject>
#include <QMap>
#include "../abstractserver.h"

class ClientSensor : public QObject
{
    Q_OBJECT
public:
    explicit ClientSensor(AbstractServer *parent);
    ~ClientSensor();

    QMap<QString, QString> *getSettings();
    virtual QString getDescriptionString();
    virtual QString getValueString();
    virtual void setEnabled(bool enabled);
    virtual void sendNow();

signals:
    void sendingInitiate(QString);

protected:
    QMap<QString,QString> *settings;
    QChar letter;
    bool is_enabled;
};

#endif // CLIENTSENSOR_H
