//
// Created by grand on 11.03.2023.
//
#pragma once

#include <QObject>
#include <QBluetoothAddress>
#include <QBluetoothServiceInfo>

QT_FORWARD_DECLARE_CLASS(QBluetoothServiceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QString)

class BTServices : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        NotContainSPP
    } BTServicesError;
    explicit BTServices(QObject *parent = nullptr);
    ~BTServices();

public slots:
            void discoveryServices(const QBluetoothAddress &address);
    void discoveryFinished();
    void serviceFind(QBluetoothServiceInfo);

signals:
    void servicesDiscovering();
    void servicesDiscovered(const QBluetoothServiceInfo & sppservice);
    void serviceDiscovered(const QBluetoothServiceInfo & service);
    void servicesError(const BTServices::BTServicesError &error);
    void logMessage(const QString &message);

private:
    void createAgent();
    void checkSPPService();

    QBluetoothServiceDiscoveryAgent *m_serviceDiscoveryAgent;
    QBluetoothAddress m_address;
};


