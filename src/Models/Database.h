//
// Created by grand on 11.03.2023.
//

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QDebug>
#include <QtCore/QCoreApplication>

#pragma once

#define BT_DEVICES_TABLE         "bt_devices"
#define BT_SERVICES_TABLE        "bt_services"
#define EEG_SESSION_TABLE        "eeg_session"
#define EEG_NORM_DATA_TABLE      "eeg_norm_data"
#define EEG_DATA_TABLE           "eeg_data"
#define EEG_ATTENTION_TABLE      "eeg_attention"
#define EEG_MEDITATION_TABLE     "eeg_meditaion"
#define EEG_POOR_SIGNAL_TABLE    "eeg_poor_signal"
#define EEG_BLINK_STRENGTH_TABLE "eeg_blinking_strength_table"

class Database: QObject {
    Q_OBJECT
public:
    /**
     * Создаёт файл базы данных или подключается к существующему
     * @brief createDb
     * @return
     */
    static bool createDb(const QString &name, bool recreate = false) {
        if(!db.isOpen()) {
            db.setDatabaseName(name);
            if (!db.open()) {
                qDebug() << "Error open database";
                QMessageBox::critical(nullptr, tr("Cannot open database"),
                                      "Unable to establish a database connection.\n"
                                      "This example needs SQLite support. Please read the Qt SQL driver documentation for information how to build it.\n"
                                      "Click «Cancel» to exit.", QMessageBox::Cancel);
                return false;
            }
            if(recreate) {
                qDebug() << "Recreate: " << name;
                for(QString table : db.tables()) {
                    query.exec(QString("DROP TABLE %1").arg(table));
                }
            }

            if(!createBTDevicesTable(BT_DEVICES_TABLE)) return false;
            if(!createBTServicesTable(BT_SERVICES_TABLE)) return false;
            if(!createSessionTable(EEG_SESSION_TABLE)) return false;
            if(!createEEGNormDataTable(EEG_DATA_TABLE)) return false;
            if(!createEEGDataTable(EEG_DATA_TABLE)) return false;
            if(!createAttentionTable(EEG_ATTENTION_TABLE)) return false;
            if(!createPoorSignalTable(EEG_POOR_SIGNAL_TABLE)) return false;
            if(!createMeditationTable(EEG_MEDITATION_TABLE)) return false;
            if(!createBlinkStrengthTable(EEG_BLINK_STRENGTH_TABLE)) return false;
        }

        return true;
    }

    static bool cleanTable(const QString &name) {
        if(!query.exec(QString("DELETE FROM %1").arg(name))) {
            qDebug() << "Error: " << query.lastError();
            return false;
        }

        return true;
    }

public:
    static QSqlDatabase db;

protected:
    static bool createBTDevicesTable(const QString &name) {
        QString sql = QString("CREATE TABLE IF NOT EXISTS %1 ("
                              "`address` VARCHAR PRIMARY KEY NOT NULL, "
                              "`name` VARCHAR, "
                              "`description` VARCHAR, "
                              "`created_at` INT, "
                              "`selected_at` INT"
                              ")"
        ).arg(name);
        if(!query.exec(sql)) {
            qDebug() << "Error: " << query.lastError();
            return false;
        }

        return true;
    }

    static bool createBTServicesTable(const QString &tableName) {
        QString sql = QString("CREATE TABLE IF NOT EXISTS %1 ("
                              "`service` VARCHAR PRIMARY KEY NOT NULL, "
                              "`device` VARCHAR, "
                              "`name` VARCHAR, "
                              "`description` VARCHAR, "
                              "`created_at` INT"
                              ")"
        ).arg(tableName);
        if(!query.exec(sql)) {
            qDebug() << "Error:" << query.lastError();
            return false;
        }

        return true;
    }

    static bool createSessionTable(const QString &tableName) {
        QString sql = QString("CREATE TABLE IF NOT EXISTS `%1`("
                              "`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "`name` VARCHAR ,"
                              "`description` TEXT, "
                              "`created_at` INT"
                              ")"
        ).arg(tableName);
        if(!query.exec(sql)) {
            return false;
        }
        return true;
    }

    static bool createPoorSignalTable(const QString &tableName) {
        QString sql = QString("CREATE TABLE IF NOT EXISTS `%1`("
                              "`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "`session_id` INTEGER"
                              "`signal` INTEGER ,"
                              "`created_at` INT"
                              ")"
        ).arg(tableName);
        if(!query.exec(sql)) {
            return false;
        }
        return true;
    }

    static bool createAttentionTable(const QString &tableName) {
        QString sql = QString("CREATE TABLE IF NOT EXISTS `%1`("
                              "`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "`session_id` INTEGER"
                              "`attention` INTEGER ,"
                              "`created_at` INT"
                              ")"
        ).arg(tableName);
        if(!query.exec(sql)) {
            return false;
        }
        return true;
    }

    static bool createMeditationTable(const QString &tableName) {
        QString sql = QString("CREATE TABLE IF NOT EXISTS `%1`("
                              "`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "`session_id` INTEGER"
                              "`meditation` INTEGER ,"
                              "`created_at` INT"
                              ")"
        ).arg(tableName);
        if(!query.exec(sql)) {
            return false;
        }
        return true;
    }


    static bool createEEGNormDataTable(const QString &tableName) {
        QString sql = QString("CREATE TABLE IF NOT EXISTS `%1`("
                              "`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "`session_id` INT, "
                              "`delta` REAL, "
                              "`theta` REAL"
                              "`low_alpha` REAL, "
                              "`high_alpha` REAL, "
                              "`low_beta` REAL, "
                              "`high_beta` REAL, "
                              "`low_gamma` REAL, "
                              "`mid_gamma` REAL, "
                              "`dt_at` INT"
                              ")"
        ).arg(tableName);
        if(!query.exec(sql)) {
            return false;
        }
        return true;
    }

    static bool createEEGDataTable(const QString &tableName) {
        QString sql = QString("CREATE TABLE IF NOT EXISTS `%1`("
                              "`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "`session_id` INT, "
                              "`delta` INTEGER, "
                              "`theta` INTEGER"
                              "`low_alpha` INTEGER, "
                              "`high_alpha` INTEGER, "
                              "`low_beta` INTEGER, "
                              "`high_beta` INTEGER, "
                              "`low_gamma` INTEGER, "
                              "`mid_gamma` INTEGER, "
                              "`dt_at` INT"
                              ")"
        ).arg(tableName);
        if(!query.exec(sql)) {
            return false;
        }
        return true;
    }

    static bool createBlinkStrengthTable(const QString &tableName) {
        QString sql = QString("CREATE TABLE IF NOT EXISTS `%1`("
                              "`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "`session_id` INTEGER"
                              "`strength` INTEGER ,"
                              "`created_at` INT"
                              ")"
        ).arg(tableName);
        if(!query.exec(sql)) {
            return false;
        }
        return true;
    }

private:
    static QSqlQuery query;
};

