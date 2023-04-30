//
// Created by grand on 11.03.2023.
//

#include "BTServices.h"

#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QBluetoothAddress>

BTServices::BTServices(QObject *parent) :
        QObject(parent)
        , m_serviceDiscoveryAgent(nullptr)
{
    createAgent();
}

BTServices::~BTServices() {
    if(m_serviceDiscoveryAgent) {
        delete m_serviceDiscoveryAgent;
    }
}

void BTServices::createAgent() {
    QBluetoothLocalDevice localDevice;
    QBluetoothAddress adapterAddress = localDevice.address();
    if(m_serviceDiscoveryAgent) {
        delete m_serviceDiscoveryAgent;
    }
    m_serviceDiscoveryAgent = new QBluetoothServiceDiscoveryAgent(adapterAddress);
    connect(m_serviceDiscoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
            this, SLOT(serviceFind(QBluetoothServiceInfo)));
    connect(m_serviceDiscoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
}

void BTServices::discoveryServices(const QBluetoothAddress &address) {
    if(!address.isNull()) {
        if(address != m_address) {
            m_serviceDiscoveryAgent->clear();
            m_address = address;
            emit servicesDiscovering();
            emit logMessage(QString("Исследуются сервисы устройства %1").arg(address.toString()));
            if(m_serviceDiscoveryAgent->isActive()) {
                m_serviceDiscoveryAgent->stop();
            }
            m_serviceDiscoveryAgent->setRemoteAddress(address);
            m_serviceDiscoveryAgent->start();
        } else if (m_serviceDiscoveryAgent->discoveredServices().length()) {
            checkSPPService();
        }
    }
}


void BTServices::serviceFind(QBluetoothServiceInfo service) {
    emit logMessage(QString("Найден сервис %1").arg(service.serviceName()));
    emit serviceDiscovered(service);
    // qDebug() << service;
}

void BTServices::checkSPPService() {
    QList<QBluetoothServiceInfo> services = m_serviceDiscoveryAgent->discoveredServices();
    if(services.length()) {
                foreach(QBluetoothServiceInfo service, services) {
                if(service.contains(0x100)) {
                    emit servicesDiscovered(service);
                    return;
                }
            }
    }
    emit servicesError(BTServicesError::NotContainSPP);
}

void BTServices::discoveryFinished() {
    emit logMessage(QString("Исследование сервисов устройства %1 завершено").arg(m_address.toString()));
    checkSPPService();
}
