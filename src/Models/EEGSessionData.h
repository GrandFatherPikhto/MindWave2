//
// Created by denis on 28.04.2023.
//
#pragma once

#include <QString>
#include <QDateTime>

class EEGSessionData {
public:
    EEGSessionData  ();
    EEGSessionData  (const QString& name, const QString &description, int id = -1, int createdAt = -1);
    EEGSessionData  (const EEGSessionData& data);
    ~EEGSessionData ();

    const EEGSessionData & operator = (const EEGSessionData &data);

    inline int getId(void) { return m_id; }
    inline QString getName(void) { return m_name; }
    inline QString getDescription(void) { return m_description; }
    inline int getCreatedAt(void) { return m_createdAt; }

    inline void setId(int id) { m_id = id; }
    inline void setName(const QString &name) { m_name = name; }
    inline void setDescription(const QString& description) { m_description = description; }
    inline void setDatetime(int dt) { m_createdAt = dt; }

private:
    int m_id;
    QString m_name;
    QString m_description;
    int m_createdAt;
};


