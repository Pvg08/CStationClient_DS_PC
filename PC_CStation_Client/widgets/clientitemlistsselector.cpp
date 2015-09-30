#include "clientitemlistsselector.h"
#include "ui_clientitemlistsselector.h"

ClientItemListsSelector::ClientItemListsSelector(QWidget *parent, QString label) :
    QWidget(parent),
    ui(new Ui::ClientItemListsSelector)
{
    ui->setupUi(this);
    ui->label->setText(label);
    items_indexes = new QVector<ClientItem*>();
}

ClientItemListsSelector::~ClientItemListsSelector()
{
    delete ui;
    delete items_indexes;
}

void ClientItemListsSelector::addClientItem(ClientItem* item)
{
    items_indexes->append(item);
    QListWidgetItem* t = new QListWidgetItem(item->getItemName(), ui->listWidget);
    t->setCheckState(Qt::Checked);
}

void ClientItemListsSelector::updateStates()
{
    for(int i=0; i<items_indexes->size(); i++) {
        if (QListWidgetItem* item = ui->listWidget->item(i)) {
            ClientItem* clitem = items_indexes->at(i);
            if (clitem && clitem->isEnabled() != (item->checkState() == Qt::Checked)) {
                clitem->setEnabled(item->checkState() == Qt::Checked);
            }
        }
    }
}

void ClientItemListsSelector::loadStatesFromSettings(QSettings *settings, QString blockCode)
{
    int i = 0;
    QListWidgetItem* t = ui->listWidget->item(i);
    while (t) {
        t->setCheckState(settings->value(blockCode+"/item"+QString::number(i), t->checkState() == Qt::Checked).toBool() ? Qt::Checked : Qt::Unchecked);
        i++;
        t = ui->listWidget->item(i);
    }
    updateStates();
}

void ClientItemListsSelector::saveStatesToSettings(QSettings *settings, QString blockCode)
{
    int i = 0;
    QListWidgetItem* t = ui->listWidget->item(i);
    while (t) {
        settings->setValue(blockCode+"/item"+QString::number(i), t->checkState() == Qt::Checked);
        i++;
        t = ui->listWidget->item(i);
    }
}

int ClientItemListsSelector::getItemIndex(QListWidgetItem *item)
{
    int index = -1;
    int i = 0;
    QListWidgetItem* t = ui->listWidget->item(i);
    while (t && index<0) {
        if (t==item) index = i;
        i++;
        t = ui->listWidget->item(i);
    }
    return index;
}

void ClientItemListsSelector::on_listWidget_itemChanged(QListWidgetItem *item)
{
    int index = getItemIndex(item);
    if (index>=0) {
        ClientItem* clitem = items_indexes->value(index, NULL);
        if (clitem && clitem->isEnabled() != (item->checkState() == Qt::Checked)) {
            clitem->setEnabled(item->checkState() == Qt::Checked);
        }
    }
}
