#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <QtCore/QCoreApplication>
#include "../Models/Database.h"

#include "../MainWindow/MainWindow.h"

#include "MindWave2.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(ORGANIZATION);
    QCoreApplication::setOrganizationDomain(ORG_DOMAIN);
    QCoreApplication::setApplicationName(APP_NAME);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = QString("%1_%2").arg(APP_NAME, QLocale(locale).name());
        if (translator.load(baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    if(!Database::createDb(DB_NAME, false)) {
        return EXIT_FAILURE;
    }

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
