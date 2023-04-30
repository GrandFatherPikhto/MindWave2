//
// Created by grand on 11.03.2023.
//
#pragma once

#include <QDialog>
#include <QComboBox>
#include <QBluetoothAddress>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QSqlError>
#include <QBluetoothLocalDevice>

#define MINDWAVE_SERVICE_UUID "00001101-0000-1000-8000-00805f9b34fb"

QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QModelIndex)
QT_FORWARD_DECLARE_CLASS(Database)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(BTDevicesModel)
QT_FORWARD_DECLARE_CLASS(QTableView)
QT_FORWARD_DECLARE_CLASS(QStatusBar)
QT_FORWARD_DECLARE_CLASS(QSettings)
QT_FORWARD_DECLARE_CLASS(ModelProxy)
QT_FORWARD_DECLARE_CLASS(TimeDelegate)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)

class DialogBT : public QDialog {
    Q_OBJECT
public:
    typedef enum {
        SCAN_MINDWAVE = 0x01,
        SCAN_AUTHORIZED_PAIRED = 0x02,
        SCAN_PAIRED = 0x03,
        SCAN_ALL = 0x04,
        SCAN_DEEP = 0x05
    } SCAN_TYPE;
    explicit DialogBT(QWidget *parent = nullptr);
    ~DialogBT();
    void getSelectAddress(QBluetoothAddress &address);
    const QBluetoothAddress & getAddress() const { return m_address; }

public slots:
    void startScan         ();
    void scanFinished      ();
    void scanError         (QBluetoothDeviceDiscoveryAgent::Error error);
    void scanDeviceUpdated (const QBluetoothDeviceInfo &info, QBluetoothDeviceInfo::Fields updatedFields);
    void scanCanceled      ();
    void addDevice         (const QBluetoothDeviceInfo &);
    void clear             ();
    void loadSettings      ();
    void storeSettings     ();
    void selectionChanged  (const QModelIndex &current, const QModelIndex &previous);

signals:
    void message(const QString &);

private:
    void      initLayout        ();
    void      initScanCombo     ();
    void      initConnections   ();
    void      initTable         ();
    void      scanStarted       ();
    void      scanStopped       ();

    void      serviceDiscovery  (const QBluetoothAddress &);
    void      saveCurrentDevice ();
    void      loadCurrentDevice ();
    void      showCurrentDevice ();

    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    Database *db;
    BTDevicesModel *m_model;
    QBluetoothAddress m_address;

    QGridLayout  *m_layout;

    QPushButton  *m_scanButton;
    QPushButton  *m_resetButton;
    QComboBox    *m_scanCombo;

    QTableView   *m_table;
    QStatusBar   *m_statusBar;
    TimeDelegate *m_timeDelegate;

public:
    const QString &deviceAddress() const;
    const QString &deviceName() const;

private:
    QString     m_deviceName;
    QString     m_deviceAddress;
    QBluetoothLocalDevice m_localDevice;
    QSettings   *m_settings;
    QDialogButtonBox *m_buttonBox;
};

