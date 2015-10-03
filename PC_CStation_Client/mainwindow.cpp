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

    listItemsActions = new ClientItemListsSelector(this, tr("Active actions:"));
    listItemsSensors = new ClientItemListsSelector(this, tr("Active sensors:"));

    QHash<QString, ClientAction *>::const_iterator i = server->clientActions()->constBegin();
    while (i != server->clientActions()->constEnd()) {
        if (i.value()) {
            listItemsActions->addClientItem(i.value());
        }
        ++i;
    }
    QHash<QString, ClientSensor *>::const_iterator j = server->clientSensors()->constBegin();
    while (j != server->clientSensors()->constEnd()) {
        if (j.value()) {
            listItemsSensors->addClientItem(j.value());
        }
        ++j;
    }

    ui->widget_clientactions->layout()->addWidget(listItemsActions);
    ui->widget_clientsensors->layout()->addWidget(listItemsSensors);

    load_settings(QCoreApplication::instance()->applicationDirPath()+"/config.cfg");
    updateServerParams();

    QObject::connect(server, SIGNAL(set_config(QString,int)), this, SLOT(set_config(QString,int)));
    QObject::connect(server, SIGNAL(error(QString)), this, SLOT(get_error(QString)));
    QObject::connect(server, SIGNAL(write_message(QString)), this, SLOT(get_message(QString)));

    ClientAction* indication_action = server->clientActions()->value("led", NULL);
    if (indication_action) {
        QObject::connect(indication_action, SIGNAL(updateState()), this, SLOT(updateIndicationState()));
    }

    ClientAction* lcd_action = server->clientActions()->value("lcd", NULL);
    if (lcd_action) {
        QObject::connect(lcd_action, SIGNAL(updateState()), this, SLOT(updateLcdState()));
    }

    server->StartServer();
}

MainWindow::~MainWindow()
{
    delete listItemsActions;
    delete listItemsSensors;
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

    listItemsActions->saveStatesToSettings(&settings, "actions");
    listItemsSensors->saveStatesToSettings(&settings, "sensors");
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

    listItemsActions->loadStatesFromSettings(&settings, "actions");
    listItemsSensors->loadStatesFromSettings(&settings, "sensors");
}

void MainWindow::get_message(QString message)
{
    ui->statusBar->showMessage(message, 10000);
}

void MainWindow::get_error(QString message)
{
    ui->statusBar->showMessage("Error: "+message, 10000);
}

void MainWindow::set_config(QString ip_addr, int ds_id)
{
    ui->lineEdit_ip->setText(ip_addr);
    ui->spinBox_id->setValue(ds_id);
    save_settings(QCoreApplication::instance()->applicationDirPath()+"/config.cfg");
    server->Reset();
}

void MainWindow::updateIndicationState()
{
    ClientAction *action = dynamic_cast<ClientAction*>(this->sender());
    if (action) {
        if (action->getSettings()->value("VALUE", "0").toInt()) {
            ui->frame_indication->setStyleSheet("background-color:rgb(50,180,255);");
        } else {
            ui->frame_indication->setStyleSheet("background-color:rgb(127,127,127);");
        }
        ui->frame_indication->repaint();
    }
}

void MainWindow::updateLcdState()
{
    ClientAction *action = dynamic_cast<ClientAction*>(this->sender());
    if (action) {
        QString msg = action->getSettings()->value("TEXT", "");
        ui->label_lcd->setText(msg);
    }
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
