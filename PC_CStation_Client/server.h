#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QNetworkSession>
#include <QtNetwork>
#include <stdlib.h>

class Server : public QObject
{
    Q_OBJECT

public:
    Server();
    ~Server();
    void Reset();
    void StartServer();
    bool SendData(QString message);

    int getRemotePort() const;
    void setRemotePort(int value);
    QString getRemoteIPAddress() const;
    void setRemoteIPAddress(const QString &value);
    int getDeviceId() const;
    void setDeviceId(int value);
    int getLocalPort() const;
    void setLocalPort(int value);

signals:
    void error(QString message);
    void write_message(QString message);

private slots:
    void sessionOpened();
    void recieveConnection();
    void recieveData();
    void clientDisconnected();
    void socketStateChanged(QAbstractSocket::SocketState state);
    void displayError(QAbstractSocket::SocketError socketError);
private:
    bool is_init_connection;
    int remote_port = 0;
    int local_port = 0;
    int device_id = 0;
    QString remoteIPAddress;
    QTcpSocket *remote_server_socket;
    QMap<quint32, QTcpSocket *> *sockets;
    QString thisIPAddress;
    QTcpServer *tcpServer;
    QNetworkSession *networkSession;

    QTcpSocket *getRemoteSocket();
    void sendSensorsInfo();
    void sendActionsInfo();
};

#endif
