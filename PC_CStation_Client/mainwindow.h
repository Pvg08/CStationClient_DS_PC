#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void get_message(QString message);
    void get_error(QString message);

    void on_pushButton_listen_clicked();

    void on_pushButton_config_clicked();

    void on_pushButton_action_pressed();

    void on_pushButton_action_released();

    void on_listWidget_sensors_itemChanged(QListWidgetItem *item);

    void on_listWidget_actions_itemChanged(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    Server* server;

    void updateServerParams();
    void save_settings(QString filename);
    void load_settings(QString filename);
    void closeEvent(QCloseEvent *event);

    int getItemIndex(QListWidget *w, QListWidgetItem *item);
};

#endif // MAINWINDOW_H
