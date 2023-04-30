//
// Created by grand on 11.03.2023.
//

#ifndef MINDWAVE2_BTREADER_H
#define MINDWAVE2_BTREADER_H

#include <QObject>
#include <QBluetoothSocket>

QT_FORWARD_DECLARE_CLASS(QBluetoothAddress)
QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceInfo)
QT_FORWARD_DECLARE_CLASS(QBluetoothServiceInfo)
QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceDiscoveryAgent)

#define DATA_SIZE 0xFF

class BTReader : public QObject
{
Q_OBJECT
public:
    explicit BTReader(QObject *parent = nullptr);
    ~BTReader();

    bool isOpen();

public slots:
    void connectBT(const QBluetoothServiceInfo & service);
    void disconnectBT();
    void readSocket();
    void errorSocket(QBluetoothSocket::SocketError error);

signals:
    void logMessage(const QString & message);
    void socketConnecting();
    void socketConnected();
    void socketRead(const QByteArray &data);
    void socketDisconnected();
    void socketError(QBluetoothSocket::SocketError error);

private:
    QBluetoothSocket *m_socket;
    QList<QBluetoothServiceInfo> m_services;
};


#endif //MINDWAVE2_BTREADER_H
