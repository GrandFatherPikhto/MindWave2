//
// Created by grand on 11.03.2023.
//

#include "Database.h"

#include <QtCore/QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>

QSqlDatabase Database::db = QSqlDatabase::addDatabase("QSQLITE");
QSqlQuery Database::query = QSqlQuery();

