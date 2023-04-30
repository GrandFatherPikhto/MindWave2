//
// Created by denis on 28.04.2023.
//
#include "EEGSession.h"
#include "../../Models/EEGSessionModel.h"
#include "../../Models/Delegates/TimeDelegate.h"

#include <QDebug>
#include <QSettings>

#define STORE_GROUP "EEGSession"

EEGSession::EEGSession(QWidget *parent) :
    QTableView(parent)
    , m_model(new EEGSessionModel(this))
    , m_settings(new QSettings(tr("EScripka"), tr("MindWave2")))
    , m_timeDelegate(new TimeDelegate(this))
{
    QTableView::setModel(m_model);
    setItemDelegateForColumn(EEGSessionModel::ColCreatedAt, m_timeDelegate);
    restoreSettings();
    m_model->select();
}

EEGSession::~EEGSession() {
    storeSettings();
}

void EEGSession::restoreSettings() {
    m_settings->beginGroup(STORE_GROUP);
    QByteArray geometry = m_settings->value("geometry", this->saveGeometry()).toByteArray();
    restoreGeometry(geometry);
    for(uint8_t i = 0; i < m_model->columnCount(); i++) {
        setColumnWidth(i, m_settings->value(QString("column%1").arg(i),
                                            QVariant(150)).toInt());
    }
    m_settings->endGroup();
}

void EEGSession::storeSettings() {
    m_settings->beginGroup(STORE_GROUP);
    m_settings->setValue("geometry", this->saveGeometry());
    for(uint8_t i = 0; i < m_model->columnCount(); i++) {
        m_settings->setValue(QString("column%1").arg(i), columnWidth(i));
    }
    m_settings->endGroup();
}

void EEGSession::updateSessionTable() {
    m_model->select();
    qDebug() << m_model->rowCount() << m_model->index(0, 0) << m_model->index(m_model->rowCount() - 1, m_model->columnCount() - 1);
    emit dataChanged(m_model->index(0, 0)
            , m_model->index(m_model->rowCount() - 1, m_model->columnCount() - 1));
}
