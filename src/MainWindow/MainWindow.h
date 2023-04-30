//
// Created by grand on 10.03.2023.
//
#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QBluetoothDeviceInfo>
#include <QMenuBar>
#include <QMenu>
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QMutex>
#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QBluetoothSocket>

#include "../Connectors/Bluetooth/BTServices.h"

QT_FORWARD_DECLARE_CLASS(EEGBarWidget)
QT_FORWARD_DECLARE_CLASS(EEGWaveWidget)
QT_FORWARD_DECLARE_CLASS(EEGLogWidget)
QT_FORWARD_DECLARE_CLASS(EEGStatusWidget)
QT_FORWARD_DECLARE_CLASS(EEGSessionWidget)
QT_FORWARD_DECLARE_CLASS(DialogBT)
QT_FORWARD_DECLARE_CLASS(DialogSession)
QT_FORWARD_DECLARE_CLASS(Bluetooth)
QT_FORWARD_DECLARE_CLASS(Generator)
QT_FORWARD_DECLARE_CLASS(TGParser)
QT_FORWARD_DECLARE_CLASS(SerialPort)

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    typedef enum {
        ConnectGenerator = 0x00,
        ConnectBluetooth = 0x01,
        ConnectSerialPort = 0x02
    } Connection;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent *) override;

private slots:
    void BtServicesDiscovering  ();
    void BtServicesError        (const BTServices::BTServicesError &error);
    void BtServiceDiscovered    (const QBluetoothServiceInfo & service);
    void BtServicesDiscovered   (const QBluetoothServiceInfo & sppservice);
    void BtSocketError          (QBluetoothSocket::SocketError error);
    void BtSocketConnecting     ();
    void BtSocketConnected      ();
    void BtSocketDisconnected   ();

    void logMessage(const QString &message);

protected slots:
    void dialogBluetooth        ();
    void openBluetooth          ();
    void openGenerator          ();
    void acceptSession          ();

protected:
    void createMenus            ();
    void createDockingWidgets   ();
    void createToolbar          ();
    void initToolbar            ();
    void initSignals            ();
    void restoreSettings        ();
    void storeSettings          ();
    void connectGenerator       ();
    void connectBluetooth       ();
    void connectSerailPort      ();
    void disconnectGenerator    ();
    void disconnectBluetooth    ();
    void disconnectSerialPort   ();

    template<class T>
    T* createDockWidget (
            const QString &name
            , const QString &windowTitle
            , const QString &menuName
            , const QString &hotKey = QString()
            , Qt::Orientation orientation = Qt::Horizontal
            , bool closable = false);
    void createOptionsWaveMenu  ();
    void createOptionsBarMenu   ();

    QGridLayout  *m_layout;
    QToolBar     *m_toolbar;
    QWidget      *m_container;
    QMenu        *m_viewMenu;
    QMenu        *m_optionsMenu;
    QMenu        *m_fileMenu;
    QMenu        *m_waveMenu;
    QMenu        *m_barMenu;
    QMenu        *m_editMenu;
    QAction      *m_connectBt;
    QAction      *m_connectGen;
    QAction      *m_optionsBt;
    QAction      *m_quit;
    QAction      *m_runAction;
    QAction      *m_runAllAction;
    QAction      *m_waveDeltaAction;
    QAction      *m_waveThetaAction;
    QAction      *m_waveLowAlphaAction;
    QAction      *m_waveHighAlphaAction;
    QAction      *m_waveLowBetaAction;
    QAction      *m_waveHighBetaAction;
    QAction      *m_waveLowGammaAction;
    QAction      *m_waveMiddleGammaAction;
    QAction      *m_barDeltaAction;
    QAction      *m_barThetaAction;
    QAction      *m_barLowAlphaAction;
    QAction      *m_barHighAlphaAction;
    QAction      *m_barLowBetaAction;
    QAction      *m_barHighBetaAction;
    QAction      *m_barLowGammaAction;
    QAction      *m_barMiddleGammaAction;
    QAction      *m_cleanAction;

    EEGBarWidget     *m_eegBar;
    EEGWaveWidget    *m_eegWave;
    EEGLogWidget     *m_eegLog;
    EEGStatusWidget  *m_eegStatus;
    EEGSessionWidget *m_eegSessions;

    DialogBT      *m_dialogBt;
    DialogSession *m_dialogSession;

    TGParser      *m_parser;
    SerialPort    *m_serialPort;
    Generator     *m_generator;
    Bluetooth     *m_bluetooth;

    QSettings     *m_settings;

    Connection     m_connection;
};

