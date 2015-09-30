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

    load_settings(QCoreApplication::instance()->applicationDirPath()+"/config.cfg");
    updateServerParams();

    QObject::connect(server, SIGNAL(set_config(QString,int)), this, SLOT(set_config(QString,int)));
    QObject::connect(server, SIGNAL(error(QString)), this, SLOT(get_error(QString)));
    QObject::connect(server, SIGNAL(write_message(QString)), this, SLOT(get_message(QString)));

    server->StartServer();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    save_settings(QCoreApplication::instance()->applicationDirPath()+"/config.cfg");
}

void MainWindow::save_settings(QString filename)
{
    QSettings settings(filename, QSettings::IniFormat);

    settings.setValue("main/server_port", ui->lineEdit_port->text().toInt());
    settings.setValue("main/server_ip", ui->lineEdit_ip->text());
    settings.setValue("main/client_port", ui->lineEdit_port_client->text().toInt());

    settings.setValue("main/ds_id", ui->spinBox_id->value());
    settings.setValue("main/sensor_interval", ui->spinBox_interval->value());

    settings.setValue("window/maximized", isMaximized());
    settings.setValue("window/minimized", isMinimized());
    if (!isMaximized() && !isMinimized()) {
        QRect gg = this->geometry();
        settings.setValue("window/left", gg.left());
        settings.setValue("window/top", gg.top());
    }

    int i = 0;
    QListWidgetItem* t = ui->listWidget_sensors->item(i);
    while (t) {
        settings.setValue("sensors/item"+QString::number(i), t->checkState() == Qt::Checked);
        i++;
        t = ui->listWidget_sensors->item(i);
    }

    i = 0;
    t = ui->listWidget_actions->item(i);
    while (t) {
        settings.setValue("actions/item"+QString::number(i), t->checkState() == Qt::Checked);
        i++;
        t = ui->listWidget_actions->item(i);
    }
}

void MainWindow::load_settings(QString filename)
{
    QSettings settings(filename, QSettings::IniFormat);

    ui->lineEdit_port->setText(QString::number(settings.value("main/server_port", ui->lineEdit_port->text()).toInt()));
    ui->lineEdit_ip->setText(settings.value("main/server_ip", ui->lineEdit_ip->text()).toString());
    ui->lineEdit_port_client->setText(QString::number(settings.value("main/client_port", ui->lineEdit_port_client->text()).toInt()));

    ui->spinBox_id->setValue(settings.value("main/ds_id", ui->spinBox_id->value()).toInt());
    ui->spinBox_interval->setValue(settings.value("main/sensor_interval", ui->spinBox_interval->value()).toInt());

    QWidget::move(settings.value("window/left", 300).toInt(), settings.value("window/top", 300).toInt());

    if (settings.value("window/maximized", false).toBool()) {
        showMaximized();
    }
    if (settings.value("window/minimized", false).toBool()) {
        showMinimized();
    }

    int i = 0;
    QListWidgetItem* t = ui->listWidget_sensors->item(i);
    while (t) {
        t->setCheckState(settings.value("sensors/item"+QString::number(i), t->checkState() == Qt::Checked).toBool() ? Qt::Checked : Qt::Unchecked);
        i++;
        t = ui->listWidget_sensors->item(i);
    }

    i = 0;
    t = ui->listWidget_actions->item(i);
    while (t) {
        t->setCheckState(settings.value("actions/item"+QString::number(i), t->checkState() == Qt::Checked).toBool() ? Qt::Checked : Qt::Unchecked);
        i++;
        t = ui->listWidget_actions->item(i);
    }
}

void MainWindow::get_message(QString message)
{
    ui->statusBar->showMessage(message, 10000);
}

void MainWindow::get_error(QString message)
{
    QMessageBox::information(this, tr("CStation"), message);
}

void MainWindow::set_config(QString ip_addr, int ds_id)
{
    ui->lineEdit_ip->setText(ip_addr);
    ui->spinBox_id->setValue(ds_id);
    save_settings(QCoreApplication::instance()->applicationDirPath()+"/config.cfg");
    server->Reset();
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
    server->setSendingInterval(ui->spinBox_interval->value());
}

void MainWindow::on_pushButton_action_pressed()
{
    ClientSensor* sensor = server->clientSensors()->value("button_activity", NULL);
    if (sensor) {
        sensor->getSettings()->insert("VALUE", "yes");
        sensor->sendNow();
    }
}

void MainWindow::on_pushButton_action_released()
{
    ClientSensor* sensor = server->clientSensors()->value("button_activity", NULL);
    if (sensor) {
        sensor->getSettings()->insert("VALUE", "no");
        sensor->sendNow();
    }
}

int MainWindow::getItemIndex(QListWidget *w, QListWidgetItem *item)
{
    int index = -1;
    int i = 0;
    QListWidgetItem* t = w->item(i);
    while (t && index<0) {
        if (t==item) index = i;
        i++;
        t = w->item(i);
    }
    return index;
}

void MainWindow::on_listWidget_sensors_itemChanged(QListWidgetItem *item)
{
    int index = getItemIndex(ui->listWidget_sensors, item);
    if (index>=0) {
        ClientSensor* sensor = server->clientSensors()->values().value(index, NULL);
        if (sensor && sensor->isEnabled() != (item->checkState() == Qt::Checked)) {
            sensor->setEnabled(item->checkState() == Qt::Checked);
        }
    }
}

void MainWindow::on_listWidget_actions_itemChanged(QListWidgetItem *item)
{
    int index = getItemIndex(ui->listWidget_actions, item);
    if (index>=0) {
        ClientAction* action = server->clientActions()->values().value(index, NULL);
        if (action && action->isEnabled() != (item->checkState() == Qt::Checked)) {
            action->setEnabled(item->checkState() == Qt::Checked);
        }
    }
}
