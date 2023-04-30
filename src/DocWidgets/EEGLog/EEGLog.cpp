//
// Created by denis on 18.04.2023.
//

#include "EEGLog.h"
#include <QCoreApplication>
#include <QDir>

EEGLog::EEGLog(QWidget *parent) :
    QPlainTextEdit(parent)
    , m_logFile(nullptr)
    , m_logOut (nullptr)
{
    QDir logDir("");
    if(!logDir.exists("./logs/")) {
        qDebug() << "Create dir " + logDir.absolutePath();
        if (!logDir.mkdir("./logs/")) {
            qDebug() << "Error create directory";
        }
    }
    m_logFile = new QFile("./logs/message.log");
    m_logFile->open(QIODevice::WriteOnly | QIODevice::Append);
    m_logOut  = new QTextStream(m_logFile);
}

EEGLog::~EEGLog() noexcept {
    m_logFile->close();
}

void EEGLog::appendMessage(const QString& text)
{
    appendPlainText(text);
    // verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
    *m_logOut << text << "\r\n"; // Logs to file
    m_logOut->flush();
    // optional if you want to see the changes
    // after appendPlainText() immediately
    // useful if you use this->appendMessage() in a loop
    QCoreApplication::processEvents();
}