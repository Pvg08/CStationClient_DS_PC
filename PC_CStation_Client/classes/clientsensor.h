#ifndef CLIENTSENSOR_H
#define CLIENTSENSOR_H

#include <QObject>
#include <QMap>

class ClientSensor : public QObject
{
    Q_OBJECT
public:
    explicit ClientSensor(QObject *parent);
    ~ClientSensor();

    QMap<QString, QString> *getSettings();
    virtual QString getDescriptionString();
    virtual QString getValueString();
    virtual void setEnabled(bool enabled);

signals:

protected:
    QMap<QString,QString> *settings;
    QChar letter;
    bool is_enabled;
};

#endif // CLIENTSENSOR_H
