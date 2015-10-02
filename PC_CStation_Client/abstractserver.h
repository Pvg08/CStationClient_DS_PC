#ifndef ABSTRACTSERVER_H
#define ABSTRACTSERVER_H

#include <QObject>
#include <QMap>

typedef QMap<QString, QString> ClientItemSettings;

class AbstractServer : public QObject
{
    Q_OBJECT

public:
    virtual void Reset() = 0;
    virtual void StartServer() = 0;
    virtual void StopServer() = 0;
    virtual void ConfigurationMode() = 0;
    virtual ClientItemSettings* GetItemSettings(QString itemname) = 0;
    virtual bool SendData(QString message) = 0;

signals:
    void error(QString message);
    void write_message(QString message);
};

#endif
