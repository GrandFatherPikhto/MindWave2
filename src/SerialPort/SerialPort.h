#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QSerialPort>

#include "../TGParser/TGParser.h"
#include "DialogSP.h"

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(TGParser *parser, QObject *parent = nullptr);
    ~SerialPort();

public slots:
    void startReceive(const DialogSP::Settings &);
    void stopReceive();
    void handleError(QSerialPort::SerialPortError);
    void writeData(const QByteArray &);
    void readData();
    void logData(bool log);

signals:
    void statusMessage(const QString &);
    void serialOpen(const QSerialPort &);
    void serialClose(const QSerialPort &);

    void sendData(const QByteArray &data);

private:
    QThread  *m_thread;
    TGParser *m_parser;
    QSerialPort *m_serial;
    QByteArray m_packet;
    bool m_log;
};

#endif // SERIALPORT_H
