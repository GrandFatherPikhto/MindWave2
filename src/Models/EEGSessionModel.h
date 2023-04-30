//
// Created by denis on 28.04.2023.
//
#pragma once

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QString>

#include "EEGSessionData.h"

class EEGSessionModel : public QSqlTableModel {
    Q_OBJECT
public:
    typedef enum {
        ColId = 0x0,
        ColName = 0x1,
        ColDescription = 0x2,
        ColCreatedAt = 0x3
    } Columns;

    explicit EEGSessionModel(QObject *parent = nullptr);
    ~EEGSessionModel() override;

    inline int id(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColId).data().toInt();
        }

        return -1;
    }

    inline QString name(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColName).data().toString();
        }

        return QString("");
    }

    inline QString description(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColDescription).data().toString();
        }

        return QString("");
    }

    inline int createdAt(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColCreatedAt).data().toInt();
        }

        return -1;
    }

    bool addData(EEGSessionData &data);
};


