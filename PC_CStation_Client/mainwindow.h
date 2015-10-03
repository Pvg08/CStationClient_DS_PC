#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>
#include "server.h"
#include "./widgets/clientitemlistsselector.h"

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
    void set_config(QString ip_addr, int ds_id);
    void updateIndicationState();
    void updateLcdState();

    void on_pushButton_listen_clicked();
    void on_pushButton_config_clicked();
    void on_pushButton_action_pressed();
    void on_pushButton_action_released();

private:
    Ui::MainWindow *ui;
    Server* server;
    ClientItemListsSelector *listItemsActions;
    ClientItemListsSelector *listItemsSensors;

    void updateServerParams();
    void save_settings(QString filename);
    void load_settings(QString filename);
    void closeEvent(QCloseEvent *event);

    int getItemIndex(QListWidget *w, QListWidgetItem *item);
};

#endif // MAINWINDOW_H
