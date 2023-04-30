//
// Created by grand on 11.03.2023.
//

#include "BTReader.h"

#include <QDebug>
#include <QBluetoothAddress>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothUuid>
#include <QtBluetooth/QBluetoothSocket>

BTReader::BTReader(QObject *parent) :
        QObject(parent)
        , m_socket(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol))
{
    connect(m_socket, SIGNAL(connected()), this, SIGNAL(socketConnected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(socketDisconnected()));
    connect(m_socket, SIGNAL(errorOccurred(QBluetoothSocket::SocketError)), this, SLOT(errorSocket(QBluetoothSocket::SocketError)));
}

BTReader::~BTReader() {
    delete m_socket;
}

void BTReader::connectBT(const QBluetoothServiceInfo & service) {
    // qDebug() << "Connect" << service.serviceUuid();
    if(m_socket->isOpen()) {
        m_socket->close();
    }
    emit socketConnecting();
    m_socket->connectToService(service);
}

void BTReader::disconnectBT() {
    if(m_socket->state() == QBluetoothSocket::SocketState::ConnectedState) {
        m_socket->close();
    }
    emit socketDisconnected();
}

void BTReader::readSocket() {
    // qDebug() << "Получены данные";
    while (m_socket->isOpen() && m_socket->canReadLine()) {
        QByteArray line = m_socket->readLine();
        emit socketRead(line);
    }
}

void BTReader::errorSocket(QBluetoothSocket::SocketError error) {
    qDebug() << error;
    emit socketError(error);
}

bool BTReader::isOpen() {
    return m_socket->isOpen();
}
