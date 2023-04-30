//
// Created by grand on 11.03.2023.
//

#include <QBluetoothDeviceInfo>
#include <QBluetoothAddress>
#include <QDateTime>
#include <QItemSelection>

#include "BTDevicesModel.h"
#include "Database.h"

BTDevicesModel::BTDevicesModel(QObject *parent) :
        QSqlTableModel(parent, Database::db)
{
    setTable(BT_DEVICES_TABLE);
    QSqlTableModel::setHeaderData(BTAddress, Qt::Horizontal, tr("Address"));
    QSqlTableModel::setHeaderData(BTName, Qt::Horizontal, tr("Name"));
    QSqlTableModel::setHeaderData(BTDescription, Qt::Horizontal, tr("Description"));
    QSqlTableModel::setHeaderData(BTCreatedAt, Qt::Horizontal, tr("Created At"));
    QSqlTableModel::setHeaderData(BTSelectedAt, Qt::Horizontal, tr("Selected At"));
    select();
}

BTDevicesModel::~BTDevicesModel() {

}

bool BTDevicesModel::addBTDevice(const QBluetoothDeviceInfo &device) {
    if (findByAddress(device.address().toString()) < 0) {
        qDebug() << "Add Device: " << device.address();
        int row = QSqlTableModel::rowCount();
        QSqlTableModel::insertRows(row, 1);
        QSqlTableModel::setData(index(row, BTAddress), device.address().toString());
        QSqlTableModel::setData(index(row, BTName), device.name());
        QSqlTableModel::setData(index(row, BTCreatedAt), QDateTime::currentSecsSinceEpoch());
        QSqlTableModel::setData(index(row, BTSelectedAt), QDateTime::currentSecsSinceEpoch());
    }
    bool res = QSqlTableModel::submitAll();
    if(res) {
        select();
//        data.setId(index(rowCount() - 1, ColId).data().toInt());
//        data.setState(EffectData::Added);
//        return data.id();
    }
    return res;
}

bool BTDevicesModel::removeBTDevice(const QBluetoothDeviceInfo &device) {
    for(int i = 0; i < QSqlTableModel::rowCount(); i++) {
        QString address;
        address = index(i, BTAddress).data().toString();
        if(address == device.address().toString()) {
            bool res = deleteRowFromTable(i);
            if(res) {
                select();
                return true;
            }
        }
    }
    return false;
}

bool BTDevicesModel::updateBTDevice(const QBluetoothDeviceInfo &device) {
    int row = findByAddress(device.address().toString());
    if (row >= 0) {
        qDebug() << "Update Device: " << device.address();
        QSqlTableModel::insertRows(row, 1);
        QSqlTableModel::setData(index(row, BTAddress), device.address().toString());
        QSqlTableModel::setData(index(row, BTName), device.name());
        QSqlTableModel::setData(index(row, BTCreatedAt), QDateTime::currentSecsSinceEpoch());
        QSqlTableModel::setData(index(row, BTSelectedAt), QDateTime::currentSecsSinceEpoch());
        select();
        return submitAll();
    }

    return false;
}

bool BTDevicesModel::selectRow(const QString &address, QItemSelection &item) {
    int row = findByAddress(address);
    if (row >= 0) {
        item = QItemSelection(index(row, BTAddress),
                index(row, BTSelectedAt));
        return true;
    }

    return false;
}

