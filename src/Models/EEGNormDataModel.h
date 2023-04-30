//
// Created by denis on 19.04.2023.
//
#pragma once

#include "../TGParser/TGParser.h"
#include "../TGParser/EEGData.h"

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <vector>

using namespace std;

class EEGNormDataModel : public QSqlTableModel {
    Q_OBJECT
public:
    typedef enum {
        ColId        = 0x00,
        ColSessionId = 0x01,
        ColDelta     = 0x02,
        ColTheta     = 0x03,
        ColLowAlpha  = 0x04,
        ColHighAlpha = 0x05,
        ColLowBeta   = 0x06,
        ColHighBeta  = 0x07,
        ColLowGamma  = 0x08,
        ColMidGamma  = 0x09,
        ColDtAt      = 0x0A
    } Columns;
    explicit EEGNormDataModel(QObject *parent = nullptr);
    ~EEGNormDataModel() override;

    inline int id(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColId).data().toInt();
        }

        return -1;
    }

    inline int sessionId(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColSessionId).data().toInt();
        }

        return -1;
    }

    inline float delta(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColDelta).data().toFloat();
        }

        return -1;
    }

    inline float theta(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColTheta).data().toFloat();
        }

        return -1;
    }

    inline float lowAlpha(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColLowAlpha).data().toFloat();
        }

        return -1;
    }

    inline float highAlpha(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColHighAlpha).data().toFloat();
        }

        return -1;
    }

    inline float lowBeta(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColLowBeta).data().toFloat();
        }

        return -1;
    }

    inline float highBeta(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColHighBeta).data().toFloat();
        }

        return -1;
    }

    inline float lowGamma (int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColLowGamma).data().toFloat();
        }

        return -1;
    }

    inline float midGamma (int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColMidGamma).data().toFloat();
        }

        return -1;
    }

    inline int dtAt(int row) {
        if(row >= 0 && row < rowCount()) {
            return index(row, ColDtAt).data().toInt();
        }

        return -1;
    }

    bool addData(EEGData &data, int sessionId);

private:

};
