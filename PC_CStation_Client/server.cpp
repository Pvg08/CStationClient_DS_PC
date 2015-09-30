#include "server.h"

Server::Server()
:   tcpServer(0), networkSession(0)
{
    remote_port = 51015;
    local_port = 51016;
    device_id = 0;
    thisIPAddress = "";
    remoteIPAddress = "127.0.0.1";
    is_init_connection = false;
    is_config_mode = false;
    tcpServer = NULL;
    networkSession = NULL;
    remote_server_socket = NULL;
    shotTimer = new QTimer(this);
    shotTimer->setInterval(100000);
    shotTimer->setSingleShot(false);
    connect(shotTimer, SIGNAL(timeout()), this, SLOT(sendingTimeout()));

    sensors = new QHash<QString, ClientSensor *>();
    actions = new QHash<QString, ClientAction *>();

    sensors->insert("activity", new ClientSensorActivity(this));
    sensors->insert("button_activity", new ClientSensorBtnActivity(this));
    //sensors->insert("mouse", new ClientSensor(this));
    //sensors->insert("keyboard", new ClientSensor(this));
    //sensors->insert("activity_direction", new ClientSensor(this));
    //sensors->insert("internet_speed", new ClientSensor(this));

    //actions->insert("tone", new ClientAction(this));
    //actions->insert("lcd", new ClientAction(this));
    actions->insert("led", new ClientActionIndication(this));
    //actions->insert("led_state", new ClientAction(this));
    actions->insert("reset", new ClientActionReset(this));
    actions->insert("config", new ClientActionConfig(this));
    //actions->insert("display_state", new ClientAction(this));
    //actions->insert("activity_direction", new ClientAction(this));

    initSensors();
}

Server::~Server()
{
    delete shotTimer;
    QHash<QString, ClientSensor *>::const_iterator i = sensors->constBegin();
    while (i != sensors->constEnd()) {
        if (i.value()) delete i.value();
        ++i;
    }
    QHash<QString, ClientAction *>::const_iterator j = actions->constBegin();
    while (j != actions->constEnd()) {
        if (j.value()) delete j.value();
        ++j;
    }
    delete sensors;
    delete actions;
    if (tcpServer) delete tcpServer;
    if (networkSession) delete networkSession;
    if (remote_server_socket) delete remote_server_socket;
}

void Server::initSensors()
{
    QHash<QString, ClientSensor *>::const_iterator i = sensors->constBegin();
    while (i != sensors->constEnd()) {
        if (i.value()) connect(i.value(), SIGNAL(sendingInitiate(QString)), this, SLOT(sensorInitiateDataSending(QString)));
        ++i;
    }
}

void Server::ResetServer(bool is_configuration)
{
    StopServer();
    is_config_mode = is_configuration;
    sessionOpen();
}

void Server::Reset()
{
    ResetServer(false);
}

void Server::ConfigurationMode()
{
    ResetServer(true);
}

void Server::StartServer()
{
    shotTimer->stop();
    is_config_mode = false;
    emit write_message(tr("Network session starting."));

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpen()));

        emit write_message(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpen();
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(recieveConnection()));
}

void Server::StopServer()
{
    shotTimer->stop();
    is_config_mode = false;
    emit write_message(tr("Stopping server."));

    if (remote_server_socket) {
        delete remote_server_socket;
        remote_server_socket = NULL;
    }
    if (tcpServer) {
        tcpServer->close();
    }
}

bool Server::SendData(QString message)
{
    bool result = false;
    QTcpSocket* tcpSocket = getRemoteSocket();
    if (tcpSocket && tcpSocket->state()==QAbstractSocket::ConnectedState && tcpSocket->isWritable()) {
        emit write_message(tr("Sending data (size=%1) to server. Content: \"%2\"").arg(message.length()).arg(message));
        tcpSocket->write(message.toLocal8Bit());
        result = tcpSocket->waitForBytesWritten();
        if (!result) {
            emit error(tr("Sending error: %1").arg(tcpSocket->errorString()));
        } else {
            tcpSocket->flush();
        }
    }
    return result;
}

void Server::sessionOpen()
{
    emit write_message(tr("Network session opened."));

    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    if (!tcpServer) tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, local_port)) {
        emit error(tr("Unable to start the server: %1.").arg(tcpServer->errorString()));
        return;
    }

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            thisIPAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (thisIPAddress.isEmpty())
        thisIPAddress = QHostAddress(QHostAddress::LocalHost).toString();

    emit write_message(tr("The server is running on IP: %1 port: %2\n").arg(thisIPAddress).arg(tcpServer->serverPort()));

    if (!is_config_mode) {
        getRemoteSocket();
        if (remote_server_socket) {
            SendData("DS=" + QString::number(device_id) + "\r\n");
            sendSensorsInfo();
            sendActionsInfo();
            shotTimer->start();
            sendingTimeout();
        }
    }
}

void Server::recieveConnection()
{
    if (is_init_connection) return;

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    QString client_ip = clientConnection->peerAddress().toString();
    quint32 client_ip_int = clientConnection->peerAddress().toIPv4Address();

    emit write_message(tr("New connection from IP: %1").arg(client_ip));

    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));

    if (client_ip_int == QHostAddress(remoteIPAddress).toIPv4Address() || is_config_mode) {
        if (remote_server_socket && clientConnection != remote_server_socket) {
            delete remote_server_socket;
        }
        remote_server_socket = clientConnection;
        connect(clientConnection, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        connect(clientConnection, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        connect(clientConnection, SIGNAL(readyRead()), this, SLOT(recieveData()));
        connect(clientConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    } else {
        clientConnection->abort();
    }
}

void Server::displayError(QAbstractSocket::SocketError socketError)
{
    QTcpSocket *clientConnection = dynamic_cast<QTcpSocket*>(this->sender());
    if (clientConnection != NULL) {
        switch (socketError) {
            case QAbstractSocket::RemoteHostClosedError:
                break;
            case QAbstractSocket::HostNotFoundError:
                emit write_message(tr("The host was not found. Please check the host name settings."));
                break;
            case QAbstractSocket::ConnectionRefusedError:
                emit write_message(tr("The connection was refused by the peer %1. Make sure the client is running, and check that the host name is correct.").arg(clientConnection->peerAddress().toString()));
                break;
            default:
                emit write_message(tr("Error"));
        }
    }
}

void Server::sendingTimeout()
{
    if (!is_init_connection && !is_config_mode) {
        getRemoteSocket();
        if (remote_server_socket) {
            QString message = "";
            QString tmpmessage = "";
            QHash<QString, ClientSensor *>::const_iterator i = sensors->constBegin();
            while (i != sensors->constEnd()) {
                if (i.value() && i.value()->isEnabled()) {
                    tmpmessage = i.value()->getValueString();
                    if (!tmpmessage.isEmpty()) {
                        if (!message.isEmpty()) message+=";";
                        message += tmpmessage;
                    }
                }
                ++i;
            }
            if (!message.isEmpty()) {
                if (getRemoteSocket()) {
                    message = "DS_V={"+message+"}";
                    SendData(message);
                }
            }
        }
    }
}

void Server::sensorInitiateDataSending(QString message)
{
    ClientSensor *sensor = dynamic_cast<ClientSensor*>(this->sender());
    if (!is_config_mode && sensor && !message.isEmpty()) {
        if (getRemoteSocket()) {
            message = "DS_V={"+message+"}";
            SendData(message);
        }
    }
}

QHash<QString, ClientAction *> *Server::clientActions()
{
    return actions;
}

QHash<QString, ClientSensor *> *Server::clientSensors()
{
    return sensors;
}

int Server::getLocalPort() const
{
    return local_port;
}

void Server::setLocalPort(int value)
{
    local_port = value;
}

void Server::setSendingInterval(unsigned seconds)
{
    shotTimer->setInterval(seconds*1000);
}

int Server::getDeviceId() const
{
    return device_id;
}

void Server::setDeviceId(int value)
{
    device_id = value;
}

QString Server::getRemoteIPAddress() const
{
    return remoteIPAddress;
}

void Server::setRemoteIPAddress(const QString &value)
{
    remoteIPAddress = value;
}

int Server::getRemotePort() const
{
    return remote_port;
}

void Server::setRemotePort(int value)
{
    remote_port = value;
}

QTcpSocket *Server::getRemoteSocket()
{
    if (remote_server_socket && remote_server_socket->state()==QAbstractSocket::ConnectedState && remote_server_socket->isWritable()) {
        return remote_server_socket;
    }
    if (remote_server_socket) delete remote_server_socket;
    is_init_connection = true;
    remote_server_socket = new QTcpSocket();
    remote_server_socket->connectToHost(QHostAddress(remoteIPAddress), remote_port, QIODevice::ReadWrite);
    if (remote_server_socket->waitForConnected(5000)) {
        connect(remote_server_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        connect(remote_server_socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        connect(remote_server_socket, SIGNAL(readyRead()), this, SLOT(recieveData()));
        connect(remote_server_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    } else {
        emit error(tr("Connect error: %1").arg(remote_server_socket->errorString()));
        delete remote_server_socket;
        remote_server_socket = NULL;
    }
    is_init_connection = false;
    return remote_server_socket;
}

void Server::sendSensorsInfo()
{
    QHash<QString, ClientSensor *>::const_iterator i = sensors->constBegin();
    while (i != sensors->constEnd()) {
        if (i.value() && i.value()->isEnabled()) {
            QString message = i.value()->getDescriptionString();
            if (!message.isEmpty()) {
                message = "DS_INFO="+message+"\r\n";
                SendData(message);
            }
        }
        ++i;
    }
}

void Server::sendActionsInfo()
{
    QHash<QString, ClientAction *>::const_iterator i = actions->constBegin();
    while (i != actions->constEnd()) {
        if (i.value() && i.value()->isEnabled()) {
            QString message = i.value()->getDescriptionString();
            if (!message.isEmpty()) {
                message = "DC_INFO="+message+"\r\n";
                SendData(message);
            }
        }
        ++i;
    }
}

void Server::recieveData()
{
    QTcpSocket* tcpSocket = remote_server_socket;
    if (tcpSocket && tcpSocket->bytesAvailable()>0) {
        int size = tcpSocket->bytesAvailable();
        QDataStream in(tcpSocket);
        in.setVersion(QDataStream::Qt_4_0);
        char *mem = new char[size];
        in.readRawData(mem, size);
        QString message = QString::fromLatin1(mem, size);
        delete mem;
        emit write_message(tr("Recieved data (size=%1) from %2. Content: \"%3\"").arg(size).arg(tcpSocket->peerAddress().toString()).arg(message));
        if (!is_config_mode) {
            QHash<QString, ClientAction *>::const_iterator i = actions->constBegin();
            while (i != actions->constEnd()) {
                if (i.value() && i.value()->isEnabled() && !i.value()->getPrefix().isEmpty() && message.startsWith(i.value()->getPrefix()+"=")) {
                    QString params_message = message;
                    params_message.replace(QRegExp("^("+i.value()->getPrefix()+")\\="), "");
                    i.value()->setParamsFromMessage(params_message);
                    i.value()->runAction();
                }
                ++i;
            }
        } else if(message.startsWith("DS_SETUP:\r\n")) {
            QStringList params_message = message.split("\r\n");
            params_message.removeFirst();
            if (!params_message.isEmpty() && params_message.length()>=4) {
                device_id = params_message.value(3, "0").toInt();
                remoteIPAddress = params_message.value(2, "");
                emit set_config(remoteIPAddress, device_id);
            }
        }
    }
}

void Server::clientDisconnected()
{
    QTcpSocket *connection = dynamic_cast<QTcpSocket*>(this->sender());
    if (connection && connection==remote_server_socket) {
        emit write_message(tr("Socket removed."));
    }
}

void Server::socketStateChanged(QAbstractSocket::SocketState state)
{
    QTcpSocket *connection = dynamic_cast<QTcpSocket*>(this->sender());
    if (connection && connection==remote_server_socket) {
        QString host_ip = connection->peerAddress().toString();
        switch(state) {
            case QAbstractSocket::HostLookupState:
            emit write_message(tr("%1 is performing a host name lookup.").arg(host_ip));
            break;
            case QAbstractSocket::ConnectingState:
            emit write_message(tr("%1 has started establishing a connection.").arg(host_ip));
            break;
            case QAbstractSocket::ConnectedState:
            emit write_message(tr("%1 established a connection.").arg(host_ip));
            break;
            case QAbstractSocket::BoundState:
            emit write_message(tr("%1 is bound to an address and port.").arg(host_ip));
            break;
            case QAbstractSocket::ClosingState:
            emit write_message(tr("%1 is closing.").arg(host_ip));
            break;
        }
    }
}
