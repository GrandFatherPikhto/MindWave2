//
// Created by denis on 19.04.2023.
//

#include "EEGNormDataModel.h"
#include "Database.h"
#include <QDateTime>

EEGNormDataModel::EEGNormDataModel(QObject *parent) :
        QSqlTableModel(parent, Database::db)
{
    setTable(EEG_DATA_TABLE);
    QSqlTableModel::setHeaderData(ColId, Qt::Horizontal, tr("Id"));
    QSqlTableModel::setHeaderData(ColSessionId, Qt::Horizontal, tr("Session Id"));
    QSqlTableModel::setHeaderData(ColDelta, Qt::Horizontal, tr("Delta"));
    QSqlTableModel::setHeaderData(ColTheta, Qt::Horizontal, tr("Theta"));
    QSqlTableModel::setHeaderData(ColLowAlpha, Qt::Horizontal, tr("Low Alpha"));
    QSqlTableModel::setHeaderData(ColHighAlpha, Qt::Horizontal, tr("High Alpha"));
    QSqlTableModel::setHeaderData(ColLowBeta, Qt::Horizontal, tr("Low Beta"));
    QSqlTableModel::setHeaderData(ColHighBeta, Qt::Horizontal, tr("High Beta"));
    QSqlTableModel::setHeaderData(ColLowGamma, Qt::Horizontal, tr("Low Gamma"));
    QSqlTableModel::setHeaderData(ColMidGamma, Qt::Horizontal, tr("Middle Gamma"));
    QSqlTableModel::setHeaderData(ColDtAt, Qt::Horizontal, tr("Datetime"));
    select();
}

EEGNormDataModel::~EEGNormDataModel() {

}

bool EEGNormDataModel::addData(EEGData &data, int sessionId) {
    int row = QSqlTableModel::rowCount();
    insertRows(row, 1);
    QSqlTableModel::setData(index(row, ColSessionId), sessionId);
    QSqlTableModel::setData(index(row, ColDelta), data.delta());
    QSqlTableModel::setData(index(row, ColTheta), data.theta());
    QSqlTableModel::setData(index(row, ColLowAlpha), data.lowAlpha());
    QSqlTableModel::setData(index(row, ColHighAlpha), data.highAlpha());
    QSqlTableModel::setData(index(row, ColLowBeta), data.lowBeta());
    QSqlTableModel::setData(index(row, ColHighBeta), data.highBeta());
    QSqlTableModel::setData(index(row, ColLowGamma), data.lowGamma());
    QSqlTableModel::setData(index(row, ColMidGamma), data.midGamma());
    QSqlTableModel::setData(index(row, ColDtAt),  QDateTime::currentSecsSinceEpoch());
    bool res = QSqlTableModel::submitAll();

    if(res) {
        select();
    }

    return res;
}
