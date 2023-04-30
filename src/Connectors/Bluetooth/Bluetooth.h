//
// Created by grand on 11.03.2023.
//
#pragma once

#include <QObject>
#include <QBluetoothSocket>

#include "BTServices.h"

QT_FORWARD_DECLARE_CLASS(TGParser)
QT_FORWARD_DECLARE_CLASS(DialogBT)
QT_FORWARD_DECLARE_CLASS(BTReader)
QT_FORWARD_DECLARE_CLASS(QBluetoothServiceInfo)

class Bluetooth : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        Disconnected,
        Connecting,
        Connected,
        DataReady,
        Error
    } BTStatus;

    explicit Bluetooth(TGParser *parser, DialogBT *dialogBt, QObject *parent = nullptr);
    ~Bluetooth();

    bool isOpen();

public slots:
    void startReceive();
    void stopReceive();
    void discoveredServices(const QBluetoothServiceInfo &sppservice);
    void readData(const QByteArray &data);

signals:
    void setStatus(const BTStatus & status);
    void logMessage(const QString & message);
    void servicesDiscovering();
    void servicesError(const BTServices::BTServicesError &error);
    void serviceDiscovered(const QBluetoothServiceInfo & service);
    void servicesDiscovered(const QBluetoothServiceInfo & sppservice);

    void socketConnecting();
    void socketConnected();
    void socketRead(const QByteArray &data);
    void socketDisconnected();
    void socketError(QBluetoothSocket::SocketError error);

private:
    TGParser   *m_parser;
    BTServices *m_btServices;
    BTReader   *m_btReader;
    DialogBT   *m_dialogBT;
};

