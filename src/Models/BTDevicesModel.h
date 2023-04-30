//
// Created by grand on 11.03.2023.
//

#include <QSqlTableModel>
#include <QString>
#include <QItemSelection>

#include "Database.h"

#pragma once

QT_FORWARD_DECLARE_CLASS(QBluetoothAddress)
QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceInfo)
QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceDiscoveryAgent)

class BTDevicesModel : public QSqlTableModel {
    Q_OBJECT
public:
    typedef enum {
        BTAddress,
        BTName,
        BTDescription,
        BTCreatedAt,
        BTSelectedAt
    } Columns;

    explicit BTDevicesModel(QObject *parent = nullptr);
    ~BTDevicesModel() override;

    inline bool address(int row, QString &address) {
        if(row >= 0 && row < rowCount()) {
            address = index(row, BTAddress).data().toString();
            return true;
        }

        return false;
    }

    inline bool name(int row, QString &name) {
        if(row < 0 || row > rowCount()) return false;
        name = index(row, BTName).data().toString();
        return true; }
    inline QString description(int row) { if(row < 0 || row > QSqlTableModel::rowCount()) return QString(""); index(row, BTDescription).data().toString(); }
    inline int createdAt(int row) { if(row < 0 || row > QSqlTableModel::rowCount()) return -1; index(row, BTCreatedAt).data().toInt(); }
    inline int selectedAt(int row) { if(row < 0 || row > QSqlTableModel::rowCount()) return -1; index(row, BTSelectedAt).data().toInt(); }

    inline bool setAddress(int row, const QString &address) {
        if(row < 0 || row >= QSqlTableModel::rowCount()) return false;
        QSqlTableModel::setData(index(row, BTAddress), QVariant(address));
        return QSqlTableModel::submitAll();
    }

    inline bool setName(int row, const QString &name) {
        if(row < 0 || row >= QSqlTableModel::rowCount()) return false;
        QSqlTableModel::setData(index(row, BTName), QVariant(name));
        return QSqlTableModel::submitAll();
    }

    inline bool setDescription(int row, const QString &description) {
        if(row < 0 || row >= QSqlTableModel::rowCount()) return false;
        QSqlTableModel::setData(index(row, BTDescription), QVariant(description));
        return QSqlTableModel::submitAll();
    }

    inline bool setCreatedAt(int row, int createdAt) {
        if(row < 0 || row >= QSqlTableModel::rowCount()) return false;
        QSqlTableModel::setData(index(row, BTCreatedAt), QVariant(createdAt));
        return QSqlTableModel::submitAll();
    }

    inline bool setSelectedAt(int row, int selectedAt) {
        if(row < 0 || row >= QSqlTableModel::rowCount()) return false;
        QSqlTableModel::setData(index(row, BTSelectedAt), QVariant(selectedAt));
        return QSqlTableModel::submitAll();
    }

    inline int findByAddress(const QString &address) {
        for(int row = 0; row < QSqlTableModel::rowCount(); row ++) {
            if (index(row, BTAddress).data().toString() == address) {
                return row;
            }
        }

        return  -1;
    }

    inline bool removeAll() {
        Database::cleanTable(BT_DEVICES_TABLE);
        select();
        return submitAll();
    }

    bool selectRow(const QString &address, QItemSelection &item);
    bool addBTDevice(const QBluetoothDeviceInfo &device);
    bool removeBTDevice(const QBluetoothDeviceInfo &device);
    bool updateBTDevice(const QBluetoothDeviceInfo &device);
};
