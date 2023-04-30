//
// Created by grand on 11.03.2023.
//

#include "DialogBT.h"

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothUuid>
#include <QUuid>
#include <QBluetoothDeviceInfo>
#include <QPushButton>
#include <QDialog>
#include <QGridLayout>
#include <QWidget>
#include <QDebug>
#include <QSettings>
#include <QTableView>
#include <QStatusBar>
#include <QBluetoothLocalDevice>
#include <QDialogButtonBox>

#include "../../Models/Database.h"
#include "../../Models/BTDevicesModel.h"
#include "../../Models/Proxy/ModelProxy.h"
#include "../../Models/Delegates/TimeDelegate.h"

DialogBT::DialogBT(QWidget *parent) :
        QDialog(parent)
        , m_layout(new QGridLayout)
        , m_discoveryAgent(new QBluetoothDeviceDiscoveryAgent())
        , db(new Database())
        , m_scanCombo(new QComboBox(this))
        , m_scanButton(new QPushButton(QIcon(":/icons/scan-start"), tr("Start Scan"), this))
        , m_resetButton(new QPushButton(QIcon(":/icons/reset"), tr("Reset"), this))
        , m_statusBar(new QStatusBar(this))
        , m_settings(new QSettings(tr("EScripka"), tr("MindWave2")))
        , m_timeDelegate(nullptr)
        , m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this))
{
    initScanCombo   ();
    initLayout      ();
    loadSettings    ();
    initConnections ();
}

DialogBT::~DialogBT()
{
    storeSettings();
    delete m_scanCombo;
    delete m_scanButton;
    delete m_layout;
    delete db;
    delete m_discoveryAgent;
}

void DialogBT::initConnections() {
    connect(m_scanButton, &QPushButton::clicked, this, &DialogBT::startScan);
    connect(m_resetButton, &QPushButton::clicked, this, &DialogBT::clear);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &DialogBT::addDevice);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &DialogBT::scanFinished);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred, this, &DialogBT::scanError);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceUpdated, this, &DialogBT::scanDeviceUpdated);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this, &DialogBT::scanCanceled);
    connect(m_table->selectionModel(), &QItemSelectionModel::currentChanged, this, &DialogBT::selectionChanged);
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(accept()));
}

void DialogBT::scanError(QBluetoothDeviceDiscoveryAgent::Error error) {
    scanStopped();
}

void DialogBT::scanDeviceUpdated(const QBluetoothDeviceInfo &info, QBluetoothDeviceInfo::Fields updatedFields) {
    m_model->updateBTDevice(info);
}

void DialogBT::startScan() {
    if (m_discoveryAgent->isActive()) {
        m_discoveryAgent->stop();
    } else {
        m_discoveryAgent->start();
        scanStarted();
    }
}

void DialogBT::scanFinished() {
    scanStopped();
}

void DialogBT::scanCanceled() {
    scanStopped();
}

void DialogBT::addDevice(const QBluetoothDeviceInfo &device) {
    qDebug() << device.address() << device.name() << device.serviceUuids() << m_localDevice.pairingStatus(device.address());
    QBluetoothUuid mindWaveUuid = QUuid::fromString(MINDWAVE_SERVICE_UUID);
    SCAN_TYPE mode = static_cast<SCAN_TYPE>(m_scanCombo->currentData().toInt());
    switch (mode) {
        case SCAN_MINDWAVE:
            if (!device.serviceUuids().isEmpty() && device.serviceUuids().contains(mindWaveUuid)) {
                m_model->addBTDevice(device);
            }
            break;
        case SCAN_AUTHORIZED_PAIRED:
            if (m_localDevice.pairingStatus(device.address()) == QBluetoothLocalDevice::AuthorizedPaired) {
                m_model->addBTDevice(device);
            }
            break;
        case SCAN_PAIRED:
            if (m_localDevice.pairingStatus(device.address()) == QBluetoothLocalDevice::Paired) {
                m_model->addBTDevice(device);
            }
            break;
        case SCAN_ALL:
            m_model->addBTDevice(device);
            break;
        case SCAN_DEEP:
            m_model->addBTDevice(device);
            break;
        default:
            m_model->addBTDevice(device);
            break;
    }
}

void DialogBT::clear() {
    m_model->removeAll();
}

void DialogBT::loadSettings() {
    QByteArray geometry = m_settings->value("BluetoothDevices/geometry", this->saveGeometry()).toByteArray();
    this->restoreGeometry(geometry);
    loadCurrentDevice();
    for(uint8_t i = 0; i < m_model->columnCount(); i++) {
        m_table->setColumnWidth(i, m_settings->value(QString("BluetoothDevices/column%1").arg(i), QVariant(150)).toInt());
    }
//    m_deviceAddress = m_settings->value("BluetoothDevices/SelectedDevice/address", QBluetoothAddress().toString()).toString();
//    m_deviceName    = m_settings->value("BluetoothDevices/SelectedDevice/name", QString("")).toString();
//    qDebug() << "Select: " << m_address;
}

void DialogBT::storeSettings() {
    m_settings->setValue("BluetoothDevices/geometry", this->saveGeometry());
    for(uint8_t i = 0; i < m_model->columnCount(); i++) {
        m_settings->setValue(QString("BluetoothDevices/column%1").arg(i), m_table->columnWidth(i));
    }
//    m_settings->setValue("BluetoothDevices/SelectedDevice/address", m_deviceAddress);
//    m_settings->setValue("BluetoothDevices/SelectedDevice/name", m_deviceName);
}

void DialogBT::selectionChanged(const QModelIndex &current, const QModelIndex &previous) {
    if (current.row() >= 0 && m_model->name(current.row(), m_deviceName) && m_model->address(current.row(), m_deviceAddress)) {
        showCurrentDevice();
        saveCurrentDevice();
    }
}

void DialogBT::initLayout() {
    m_layout->addWidget(m_scanButton, 0, 0);
    m_layout->addWidget(m_resetButton, 0, 1);
    m_layout->addWidget(m_scanCombo, 0, 2);
    initTable ();
    m_layout->addWidget(m_table, 1, 0, 1, 3);
    m_layout->addWidget(m_statusBar, 2, 0, 1, 3);
    m_layout->addWidget(m_buttonBox, 3, 0, 1, 3);
    setLayout(m_layout);
}

void DialogBT::initScanCombo() {
    m_scanCombo->addItem(tr("MindWave Paired"), SCAN_MINDWAVE);
    m_scanCombo->addItem(tr("Authorized & Paired"), SCAN_AUTHORIZED_PAIRED);
    m_scanCombo->addItem(tr("Paired"), SCAN_PAIRED);
    m_scanCombo->addItem(tr("All"), SCAN_ALL);
    m_scanCombo->addItem(tr("Deep"), SCAN_DEEP);
}

/** Связывает базу данных и таблицу устройств
 * @brief DialogBT::initModel
 * https://doc.qt.io/qt-5/qtableview.html
 * https://doc.qt.io/qt-5/qsqlrelationaltablemodel.html
 * https://doc.qt.io/qt-5/qsqltablemodel.html
 */
void DialogBT::initTable() {
    m_table = new QTableView(this);
    m_timeDelegate = new TimeDelegate(m_table);

    m_model = new BTDevicesModel(this);
    m_table->setItemDelegateForColumn(BTDevicesModel::BTSelectedAt, m_timeDelegate);
    m_table->setItemDelegateForColumn(BTDevicesModel::BTCreatedAt, m_timeDelegate);
    m_table->setModel(m_model);
    m_table->setSortingEnabled(true);

    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_model->select ();
}

void DialogBT::scanStarted () {
    m_scanButton->setText(tr("Stop Scan"));
    m_resetButton->setEnabled(false);
    m_scanButton->setIcon(QIcon(":/icons/scan-stop"));
}

void DialogBT::scanStopped () {
    m_scanButton->setText(tr("Start Scan"));
    m_resetButton->setEnabled(true);
    m_scanButton->setIcon(QIcon(":/icons/scan-start"));
}

const QString &DialogBT::deviceAddress() const {
    return m_deviceAddress;
}

const QString &DialogBT::deviceName() const {
    return m_deviceName;
}

void DialogBT::saveCurrentDevice() {
    m_settings->setValue("BluetoothDevices/CurrentDevice/address", m_deviceAddress);
    m_settings->setValue("BluetoothDevices/CurrentDevice/name", m_deviceName);
    m_address = QBluetoothAddress(m_deviceAddress);
}

void DialogBT::loadCurrentDevice() {
    m_deviceAddress = m_settings->value("BluetoothDevices/CurrentDevice/address").toString();
    m_deviceName    = m_settings->value("BluetoothDevices/CurrentDevice/name").toString();
    m_address       = QBluetoothAddress(m_deviceAddress);
    int row         = m_model->findByAddress(m_deviceAddress);

    if (row >= 0) {
        showCurrentDevice ();
        QItemSelection selection;
        if (m_model->selectRow(m_deviceAddress, selection)) {
            m_table->selectionModel()->select(selection, QItemSelectionModel::Select);
            // qDebug() << "Selected:" << m_deviceAddress << m_deviceName << row << selection;
        }
    }
}

void DialogBT::showCurrentDevice() {
    m_statusBar->showMessage(QString(tr("Address: %1, Name: %2")).arg(m_deviceAddress, m_deviceName));
}