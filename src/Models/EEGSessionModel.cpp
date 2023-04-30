//
// Created by denis on 28.04.2023.
//
#include <QDateTime>
#include "EEGSessionModel.h"
#include "Database.h"

EEGSessionModel::EEGSessionModel(QObject *parent) :
    QSqlTableModel(parent, Database::db)
{
    setTable(EEG_SESSION_TABLE);
    setHeaderData(ColId, Qt::Horizontal, tr("Id"));
    setHeaderData(ColName, Qt::Horizontal, tr("Name"));
    setHeaderData(ColDescription, Qt::Horizontal, tr("Description"));
    setHeaderData(ColCreatedAt, Qt::Horizontal, tr("Created At"));
    select();
}

EEGSessionModel::~EEGSessionModel() {

}

bool EEGSessionModel::addData(EEGSessionData &data) {
    int row = rowCount();
    insertRows(row, 1);

    setData(index(row, ColName), data.getName());
    setData(index(row, ColDescription), data.getDescription());
    if (data.getCreatedAt() < 0) {
        data.setDatetime(QDateTime::currentSecsSinceEpoch());
    }
    setData(index(row, ColCreatedAt), data.getCreatedAt());

    bool res = submitAll();
    if(res) {
        select();
        data.setId(id(row));
    }

    return res;
}
