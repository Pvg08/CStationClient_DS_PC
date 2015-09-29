#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(width(), height());
    server = new Server();

    server->setRemotePort(ui->lineEdit_port->text().toInt());
    server->setLocalPort(ui->lineEdit_port_client->text().toInt());
    server->setRemoteIPAddress(ui->lineEdit_ip->text());
    server->setDeviceId(ui->spinBox_id->value());
    server->StartServer();
    QObject::connect(server, SIGNAL(error(QString)), this, SLOT(get_error(QString)));
    QObject::connect(server, SIGNAL(write_message(QString)), this, SLOT(get_message(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}

void MainWindow::get_message(QString message)
{
    ui->statusBar->showMessage(message, 10000);
}

void MainWindow::get_error(QString message)
{
    QMessageBox::information(this, tr("CStation"), message);
}

void MainWindow::on_pushButton_listen_clicked()
{
    updateServerParams();
    server->Reset();
}

void MainWindow::on_pushButton_config_clicked()
{
    updateServerParams();
    server->ConfigurationMode();
}

void MainWindow::updateServerParams()
{
    server->setRemotePort(ui->lineEdit_port->text().toInt());
    server->setLocalPort(ui->lineEdit_port_client->text().toInt());
    server->setRemoteIPAddress(ui->lineEdit_ip->text());
    server->setDeviceId(ui->spinBox_id->value());
}
