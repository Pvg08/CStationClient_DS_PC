#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QNetworkSession>
#include <QtNetwork>
#include <stdlib.h>
#include "./abstractserver.h"
#include "./classes/clientaction.h"
#include "./classes/clientsensor.h"
#include "./classes/clientSensors/clientsensoractivity.h"
#include "./classes/clientSensors/clientsensorbtnactivity.h"
#include "./classes/clientActions/clientactiontone.h"
#include "./classes/clientActions/clientactionlcd.h"
#include "./classes/clientActions/clientactionreset.h"
#include "./classes/clientActions/clientactionconfig.h"
#include "./classes/clientActions/clientactionindication.h"
#include "./classes/clientActions/clientactionindicationstate.h"
#include "./classes/clientActions/clientactionsetdisplaystate.h"

class Server : public AbstractServer
{
    Q_OBJECT

public:
    Server();
    ~Server();
    void Reset();
    void StartServer();
    void StopServer();
    void ConfigurationMode();
    bool SendData(QString message);

    ClientItemSettings* GetItemSettings(QString itemname);
    bool RunAction(QString action_name);

    int getRemotePort() const;
    void setRemotePort(int value);
    QString getRemoteIPAddress() const;
    void setRemoteIPAddress(const QString &value);
    int getDeviceId() const;
    void setDeviceId(int value);
    int getLocalPort() const;
    void setLocalPort(int value);
    void setSendingInterval(unsigned seconds);

    QHash<QString, ClientSensor *> *clientSensors();
    QHash<QString, ClientAction *> *clientActions();

signals:
    void set_config(QString, int);
    void beforeSensorsSending();

private slots:
    void sessionOpen();
    void recieveConnection();
    void recieveData();
    void clientDisconnected();
    void socketStateChanged(QAbstractSocket::SocketState state);
    void displayError(QAbstractSocket::SocketError socketError);
    void sendingTimeout();
    void connectionTimeout();
    void sensorInitiateDataSending(QString message);

private:
    bool is_init_connection;
    bool is_config_mode;
    int remote_port = 0;
    int local_port = 0;
    int device_id = 0;
    QString remoteIPAddress;
    QTcpSocket *remote_server_socket;
    QMap<quint32, QTcpSocket *> *sockets;
    QHash<QString, ClientSensor *> *sensors;
    QHash<QString, ClientAction *> *actions;

    QString thisIPAddress;
    QTcpServer *tcpServer;
    QNetworkSession *networkSession;

    QTimer *shotTimer;
    QTimer *connectionTimer;

    QTcpSocket *getRemoteSocket();
    void sendSensorsInfo();
    void sendActionsInfo();
    void initSensors();
    void ResetServer(bool is_configuration);
};

#endif
