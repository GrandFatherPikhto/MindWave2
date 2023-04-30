//
// Created by denis on 28.04.2023.
//
#include "EEGSessionData.h"

#include <QDateTime>

EEGSessionData::EEGSessionData(const QString &name, const QString &description, int id, int createdAt) {
    m_id          = id;
    m_name        = m_name;
    m_description = m_description;
    if (createdAt < 0 ) {
        m_createdAt = QDateTime::currentSecsSinceEpoch();
    } else {
        m_createdAt = m_createdAt;
    }
}

EEGSessionData::EEGSessionData(const EEGSessionData &data) {
    m_id          = data.m_id;
    m_name        = data.m_name;
    m_description = data.m_description;
    m_createdAt   = data.m_createdAt;
}

EEGSessionData::~EEGSessionData() {

}

const EEGSessionData &EEGSessionData::operator=(const EEGSessionData &data) {
    m_id          = data.m_id;
    m_name        = data.m_name;
    m_description = data.m_description;
    m_createdAt   = data.m_createdAt;

    return *this;
}

EEGSessionData::EEGSessionData() {
    m_id   = -1;
    m_name = QString(QDateTime::currentDateTime().toString());
    m_description = QString("");
    m_createdAt   = QDateTime::currentSecsSinceEpoch();
}
