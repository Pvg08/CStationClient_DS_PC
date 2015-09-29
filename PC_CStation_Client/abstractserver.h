#ifndef ABSTRACTSERVER_H
#define ABSTRACTSERVER_H

#include <QObject>

class AbstractServer : public QObject
{
    Q_OBJECT

public:
    virtual void Reset() = 0;
    virtual void StartServer() = 0;
    virtual void ConfigurationMode() = 0;
    virtual bool SendData(QString message) = 0;

signals:
    void error(QString message);
    void write_message(QString message);
};

#endif
