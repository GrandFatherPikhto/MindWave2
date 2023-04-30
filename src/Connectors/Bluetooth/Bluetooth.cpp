//
// Created by grand on 11.03.2023.
//
#include "Bluetooth.h"

#include <QBluetoothServiceInfo>

#include "BTReader.h"
#include "DialogBT.h"
#include "../../TGParser/TGParser.h"

Bluetooth::Bluetooth(TGParser *parser, DialogBT *dialogBT, QObject *parent) :
        QObject(parent)
        , m_parser(parser)
        , m_btServices(new BTServices)
        , m_btReader(new BTReader)
        , m_dialogBT(dialogBT)
{
    connect(m_btServices, SIGNAL(servicesDiscovering()), this, SIGNAL(servicesDiscovering()));
    connect(m_btServices, SIGNAL(servicesDiscovered(QBluetoothServiceInfo)), this, SIGNAL(servicesDiscovered(QBluetoothServiceInfo)));
    connect(m_btServices, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)), this, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)));
    connect(m_btServices, SIGNAL(servicesDiscovered(QBluetoothServiceInfo)), this, SLOT(discoveredServices(QBluetoothServiceInfo)));
    connect(m_btServices, SIGNAL(servicesError(BTServices::BTServicesError)), this, SIGNAL(servicesError(BTServices::BTServicesError)));
    connect(m_btServices, SIGNAL(logMessage(QString)), this, SIGNAL(logMessage(QString)));

    connect(m_btReader, SIGNAL(socketConnecting()), this, SIGNAL(socketConnecting()));
    connect(m_btReader, SIGNAL(socketConnected()), this, SIGNAL(socketConnected()));
    connect(m_btReader, SIGNAL(socketRead(QByteArray)), this, SIGNAL(socketRead(QByteArray)));
    connect(m_btReader, SIGNAL(socketRead(QByteArray)), this, SLOT(readData(QByteArray)));
    connect(m_btReader, SIGNAL(socketDisconnected()), this, SIGNAL(socketDisconnected()));
    connect(m_btReader, SIGNAL(socketError(QBluetoothSocket::SocketError)), this, SIGNAL(socketError(QBluetoothSocket::SocketError)));
}

Bluetooth::~Bluetooth() {
    delete m_btServices;
    delete m_btReader;
}

void Bluetooth::startReceive() {
    // m_bluetooth->startReceive(m_dialogBT->settings);
    QBluetoothAddress address = m_dialogBT->getAddress();
    qDebug() << address;
    emit servicesDiscovering();
    emit m_btServices->discoveryServices(address);
}

void Bluetooth::stopReceive() {
    emit m_btReader->disconnectBT();
}


void Bluetooth::discoveredServices(const QBluetoothServiceInfo &sppservice) {
    m_btReader->connectBT(sppservice);
}

void Bluetooth::readData(const QByteArray & data) {
    foreach(uint8_t bt, data) {
        m_parser->parseNext(bt);
    }
}

bool Bluetooth::isOpen() {
    return m_btReader->isOpen();
}

