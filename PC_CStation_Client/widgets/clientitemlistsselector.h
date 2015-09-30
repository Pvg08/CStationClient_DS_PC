#ifndef CLIENTITEMLISTSSELECTOR_H
#define CLIENTITEMLISTSSELECTOR_H

#include <QListWidgetItem>
#include <QVector>
#include <QSettings>
#include "../classes/clientitem.h"

namespace Ui {
class ClientItemListsSelector;
}

class ClientItemListsSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ClientItemListsSelector(QWidget *parent, QString label);
    ~ClientItemListsSelector();

    void addClientItem(ClientItem* item);
    void updateStates();

    void loadStatesFromSettings(QSettings *settings, QString blockCode);
    void saveStatesToSettings(QSettings *settings, QString blockCode);

private slots:
    void on_listWidget_itemChanged(QListWidgetItem *item);

private:
    Ui::ClientItemListsSelector *ui;
    QVector<ClientItem*> *items_indexes;

    int getItemIndex(QListWidgetItem *item);
};

#endif // CLIENTITEMLISTSSELECTOR_H
