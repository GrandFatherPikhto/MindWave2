//
// Created by grand on 10.03.2023.
//

#include "MainWindow.h"
#include "../DocWidgets/EEGBar/EEGBarWidget.h"
#include "../DocWidgets/EEGWave/EEGWaveWidget.h"
#include "../DocWidgets/EEGLog/EEGLogWidget.h"
#include "../DocWidgets/EEGStatus/EEGStatusWidget.h"
#include "../Connectors/Bluetooth/DialogBT.h"
#include "../Connectors/Bluetooth/Bluetooth.h"
#include "../Connectors/Generator/Generator.h"
#include "../TGParser/TGParser.h"
#include "../SerialPort/SerialPort.h"
#include "../DocWidgets/EEGBar/EEGBar.h"
#include "../DocWidgets/EEGWave/EEGWave.h"
#include "../DocWidgets/EEGLog/EEGLog.h"
#include "../DocWidgets/EEGStatus/EEGStatus.h"
#include "../DocWidgets/EEGSessions/EEGSessionWidget.h"
#include "../Models/EEGSessionModel.h"
#include "../DocWidgets/EEGSessions/EEGSession.h"
#include "DialogSession.h"

#include <QDockWidget>
#include <QStatusBar>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , m_layout(nullptr)
        , m_toolbar(nullptr)
        , m_container(nullptr)
        , m_eegBar(nullptr)
        , m_eegLog(nullptr)
        , m_eegWave(nullptr)
        , m_eegStatus(nullptr)
        , m_eegSessions(nullptr)
        , m_viewMenu(nullptr)
        , m_dialogBt(new DialogBT(this))
        , m_dialogSession(new DialogSession(this))
        , m_parser(new TGParser)
        , m_generator(new Generator(m_parser))
        , m_serialPort(new SerialPort(m_parser))
        , m_bluetooth(new Bluetooth(m_parser, m_dialogBt, this))
        , m_settings(new QSettings(tr("EScripka"), tr("MindWave2")))
{
    setWindowIcon(QIcon(":/icons/eeg"));

    m_container = new QWidget(this);
    m_layout = new QGridLayout(m_container);

    m_container->setLayout(m_layout);
    m_container->setFixedWidth(0);
    m_container->setFixedHeight(0);

    setCentralWidget(m_container);

    createDockingWidgets();
    createToolbar();

    initToolbar();

    createMenus();

    m_dialogBt = new DialogBT(this);

    statusBar()->showMessage("Test");

    initSignals();

    restoreSettings();
}

MainWindow::~MainWindow() noexcept {
    // delete m_toolbar;
    // delete m_layout;
    // delete m_container;
}

void MainWindow::initSignals() {
    connect(m_bluetooth, SIGNAL(servicesError(const BTServices::BTServicesError &)),
            this, SLOT(BtServicesError(const BTServices::BTServicesError &)));
    connect(m_bluetooth, SIGNAL(serviceDiscovered(const QBluetoothServiceInfo &)),
            this, SLOT(BtServiceDiscovered(const QBluetoothServiceInfo &)));
    connect(m_bluetooth, SIGNAL(servicesDiscovered(const QBluetoothServiceInfo &)),
            this, SLOT(BtServicesDiscovered(const QBluetoothServiceInfo &)));
    connect(m_bluetooth, SIGNAL(servicesDiscovering()),
            this, SLOT(BtServicesDiscovering()));
    connect(m_bluetooth, SIGNAL(socketError(QBluetoothSocket::SocketError)),
            this, SLOT(BtSocketError(QBluetoothSocket::SocketError)));

    connect(m_bluetooth, SIGNAL(socketConnected()), this, SLOT(BtSocketConnected()));
    connect(m_bluetooth, SIGNAL(socketConnecting()), this, SLOT(BtSocketConnecting()));
    connect(m_bluetooth, SIGNAL(socketDisconnected()), this, SLOT(BtSocketDisconnected()));

    connect(m_parser, SIGNAL(setPoorSignal(uint8_t)),
            m_eegWave->getWidget(), SLOT(setPoorSignal(uint8_t)));

    connect(m_parser, SIGNAL(setEegAsicPower(EEGData &)),
            m_eegBar->getWidget(), SLOT(setEegAsicPower(EEGData &)));
    connect(m_parser, SIGNAL(setEegAsicPower(EEGData &)),
            m_eegWave->getWidget(), SLOT(setEegAsicPower(EEGData &)));

    connect(m_parser, SIGNAL(setAttention(uint8_t)),
            m_eegStatus->getWidget(), SLOT(setAttention(uint8_t)));
    connect(m_parser, SIGNAL(setMeditation(uint8_t)),
            m_eegStatus->getWidget(), SLOT(setMeditation(uint8_t)));
    connect(m_parser, SIGNAL(setPoorSignal(uint8_t)),
            m_eegStatus->getWidget(), SLOT(setPoorSignal(uint8_t)));
    connect(m_parser, SIGNAL(setBlinkStrength(uint8_t)),
            m_eegStatus->getWidget(), SLOT(setBlinkStrength(uint8_t)));

    connect(m_dialogSession, SIGNAL(accepted()), this, SLOT(acceptSession()));
}

void MainWindow::createMenus() {
    m_fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addSeparator();
    m_quit       = m_fileMenu->addAction(tr("&Quit"), QKeySequence("Ctrl+Q"), this, &MainWindow::close);
    m_quit->setIcon(QIcon(":/icons/quit"));

    m_editMenu    = menuBar()->addMenu(tr("&Edit"));
    m_cleanAction = m_editMenu->addAction(tr("C&lean"),
                                          QKeySequence("Ctrl+L"), m_eegWave->getWidget(), SLOT(clean()));

    m_optionsMenu = menuBar()->addMenu(tr("Options"));
    m_optionsBt   = m_optionsMenu->addAction(tr("Bluetooth"),
                                            QKeySequence("Ctrl+B"), this, &MainWindow::dialogBluetooth);
    m_connectBt  = m_optionsMenu->addAction(tr("Connect Bluetooth"),
                                            QKeySequence("Ctrl+T"), this, &MainWindow::openBluetooth);
    m_connectGen = m_optionsMenu->addAction(tr("Connect Generator"),
                                            QKeySequence("Ctrl+G"), this, SLOT(openGenerator()));

    m_optionsMenu->addSeparator();

    createOptionsWaveMenu ();
    createOptionsBarMenu  ();

}

void MainWindow::createOptionsWaveMenu() {
    m_waveMenu = m_optionsMenu->addMenu(tr("Wave"));
    m_waveDeltaAction = m_waveMenu->addAction(tr("Delta"),
                                              QKeySequence("Ctrl+1"),
                                              m_eegWave->getWidget(), SLOT(enableDelta(bool)));
    m_waveDeltaAction->setCheckable(true);

    m_waveThetaAction = m_waveMenu->addAction(tr("Theta"),
                                              QKeySequence("Ctrl+2"),
                                              m_eegWave->getWidget(), SLOT(enableTheta(bool)));
    m_waveThetaAction->setCheckable(true);

    m_waveLowAlphaAction = m_waveMenu->addAction(tr("Low Alpha"),
                                                 QKeySequence("Ctrl+3"),
                                                 m_eegWave->getWidget(), SLOT(enableLowAlpha(bool)));
    m_waveLowAlphaAction->setCheckable(true);

    m_waveHighAlphaAction = m_waveMenu->addAction(tr("High Alpha"),
                                                  QKeySequence("Ctrl+4"),
                                                  m_eegWave->getWidget(), SLOT(enableHighAlpha(bool)));
    m_waveHighAlphaAction->setCheckable(true);

    m_waveLowBetaAction = m_waveMenu->addAction(tr("Low Beta"),
                                                QKeySequence("Ctrl+5"),
                                                m_eegWave->getWidget(), SLOT(enableLowBeta(bool)));
    m_waveLowBetaAction->setCheckable(true);

    m_waveHighBetaAction = m_waveMenu->addAction(tr("High Beta"),
                                                 QKeySequence("Ctrl+6"),
                                                 m_eegWave->getWidget(), SLOT(enableHighBeta(bool)));
    m_waveHighBetaAction->setCheckable(true);

    m_waveLowGammaAction = m_waveMenu->addAction(tr("Low Gamma"),
                                                 QKeySequence("Ctrl+7"),
                                                 m_eegWave->getWidget(), SLOT(enableLowGamma(bool)));
    m_waveLowGammaAction->setCheckable(true);

    m_waveMiddleGammaAction = m_waveMenu->addAction(tr("Middle Gamma"),
                                                    QKeySequence("Ctrl+8"),
                                                    m_eegWave->getWidget(), SLOT(enableMiddleGamma(bool)));
    m_waveMiddleGammaAction->setCheckable(true);
}

void MainWindow::createOptionsBarMenu() {
    m_barMenu = m_optionsMenu->addMenu(tr("Bar"));
    m_barDeltaAction = m_barMenu->addAction(tr("Delta"),
                                            QKeySequence("Alt+1"),
                                            m_eegBar->getWidget(), SLOT(enableDelta(bool)));
    m_barDeltaAction->setCheckable(true);

    m_barThetaAction = m_barMenu->addAction(tr("Theta"),
                                            QKeySequence("Alt+2"),
                                            m_eegBar->getWidget(), SLOT(enableTheta(bool)));
    m_barThetaAction->setCheckable(true);

    m_barLowAlphaAction = m_barMenu->addAction(tr("Low Alpha"),
                                               QKeySequence("Alt+3"),
                                               m_eegBar->getWidget(), SLOT(enableLowAlpha(bool)));
    m_barLowAlphaAction->setCheckable(true);

    m_barHighAlphaAction = m_barMenu->addAction(tr("High Alpha"),
                                                QKeySequence("Alt+4"),
                                                m_eegBar->getWidget(), SLOT(enableHighAlpha(bool)));
    m_barHighAlphaAction->setCheckable(true);

    m_barLowBetaAction = m_barMenu->addAction(tr("Low Beta"),
                                              QKeySequence("Alt+5"),
                                              m_eegBar->getWidget(), SLOT(enableLowBeta(bool)));
    m_barLowBetaAction->setCheckable(true);

    m_barHighBetaAction = m_barMenu->addAction(tr("High Beta"),
                                               QKeySequence("Alt+6"),
                                               m_eegBar->getWidget(), SLOT(enableHighBeta(bool)));
    m_barHighBetaAction->setCheckable(true);

    m_barLowGammaAction = m_barMenu->addAction(tr("Low Gamma"),
                                               QKeySequence("Alt+7"),
                                               m_eegBar->getWidget(), SLOT(enableLowGamma(bool)));
    m_barLowGammaAction->setCheckable(true);

    m_barMiddleGammaAction = m_barMenu->addAction(tr("Middle Gamma"),
                                                  QKeySequence("Alt+8"),
                                                  m_eegBar->getWidget(), SLOT(enableMiddleGamma(bool)));
    m_barMiddleGammaAction->setCheckable(true);
}

void MainWindow::createDockingWidgets() {
    m_eegBar    = createDockWidget<EEGBarWidget>("EEGBarWidget",
                                                 EEGBarWidget::tr("EEG Bar Chart"), tr("EEG&Bar"), "", Qt::Vertical);
    m_eegWave   = createDockWidget<EEGWaveWidget>("EEGWaveWidget",
                                                  EEGWaveWidget::tr("EEG Wave Chart"), tr("&EEG&Wave"), "", Qt::Vertical);
    m_eegLog    = createDockWidget<EEGLogWidget>("EEGLogWidget",
                                                 EEGLogWidget::tr("EEG Log"), tr("EEG&Log"));
    m_eegStatus = createDockWidget<EEGStatusWidget>("EEGStatusWidget",
                                                    EEGStatusWidget::tr("EEG Status"), tr("&EEGStat&us"));
    m_eegSessions = createDockWidget<EEGSessionWidget>("EEGSessionWidget",
                                                       EEGSessionWidget::tr("EEG Sessions"), tr("&EEGSess&ions"));
}

void MainWindow::createToolbar() {

}

void MainWindow::initToolbar() {

}

template<class T>
T* MainWindow::createDockWidget (
        const QString &name
        , const QString &windowTitle
        , const QString &menuName
        , const QString &hotKey
        , Qt::Orientation orientation
        , bool closable) {
    T *widget = new T(name, windowTitle, this);

    widget->setObjectName(name);
    addDockWidget(Qt::LeftDockWidgetArea, widget, orientation);
    if(!closable) {
        widget->setFeatures(widget->features() & ~QDockWidget::DockWidgetClosable);
    }
    widget->setAllowedAreas(Qt::LeftDockWidgetArea
                            | Qt::RightDockWidgetArea
                            | Qt::TopDockWidgetArea
                            | Qt::BottomDockWidgetArea);

    return widget;
}

void MainWindow::storeSettings() {
    m_settings->beginGroup("MainWindow");
    QByteArray state = saveState();
    m_settings->setValue("state", state);
    QByteArray geom  = saveGeometry();
    m_settings->setValue("geometry", geom);

    m_settings->setValue("WaveDelta", m_waveDeltaAction->isChecked());
    m_settings->setValue("WaveTheta", m_waveThetaAction->isChecked());
    m_settings->setValue("WaveLowAlpha", m_waveLowAlphaAction->isChecked());
    m_settings->setValue("WaveHighAlpha", m_waveHighAlphaAction->isChecked());
    m_settings->setValue("WaveLowBeta", m_waveLowBetaAction->isChecked());
    m_settings->setValue("WaveHighBeta", m_waveHighBetaAction->isChecked());
    m_settings->setValue("WaveLowGamma", m_waveLowGammaAction->isChecked());
    m_settings->setValue("WaveMiddleGamma", m_waveMiddleGammaAction->isChecked());

    m_settings->setValue("BarDelta", m_barDeltaAction->isChecked());
    m_settings->setValue("BarTheta", m_barThetaAction->isChecked());
    m_settings->setValue("BarLowAlpha", m_barLowAlphaAction->isChecked());
    m_settings->setValue("BarHighAlpha", m_barHighAlphaAction->isChecked());
    m_settings->setValue("BarLowBeta", m_barLowBetaAction->isChecked());
    m_settings->setValue("BarHighBeta", m_barHighBetaAction->isChecked());
    m_settings->setValue("BarLowGamma", m_barLowGammaAction->isChecked());
    m_settings->setValue("BarMiddleGamma", m_barMiddleGammaAction->isChecked());

    m_settings->endGroup();
}

void MainWindow::restoreSettings() {
    m_settings->beginGroup("MainWindow");
    QByteArray geom  = m_settings->value("geometry").toByteArray();
    restoreGeometry(geom);
    QByteArray state = m_settings->value("state").toByteArray();
    restoreState(state);

    m_waveDeltaAction->setChecked(m_settings->value("WaveDelta", true).toBool());
    m_waveThetaAction->setChecked(m_settings->value("WaveTheta", true).toBool());
    m_waveLowAlphaAction->setChecked(m_settings->value("WaveLowAlpha", true).toBool());
    m_waveHighAlphaAction->setChecked(m_settings->value("WaveHighAlpha", true).toBool());
    m_waveLowBetaAction->setChecked(m_settings->value("WaveLowBeta", true).toBool());
    m_waveHighBetaAction->setChecked(m_settings->value("WaveHighBeta", true).toBool());
    m_waveLowGammaAction->setChecked(m_settings->value("WaveLowGamma", true).toBool());
    m_waveMiddleGammaAction->setChecked(m_settings->value("WaveMiddleGamma", true).toBool());

    m_barDeltaAction->setChecked(m_settings->value("BarDelta", true).toBool());
    m_barThetaAction->setChecked(m_settings->value("BarTheta", true).toBool());
    m_barLowAlphaAction->setChecked(m_settings->value("BarLowAlpha", true).toBool());
    m_barHighAlphaAction->setChecked(m_settings->value("BarHighAlpha", true).toBool());
    m_barLowBetaAction->setChecked(m_settings->value("BarLowBeta", true).toBool());
    m_barHighBetaAction->setChecked(m_settings->value("BarHighBeta", true).toBool());
    m_barLowGammaAction->setChecked(m_settings->value("BarLowGamma", true).toBool());
    m_barMiddleGammaAction->setChecked(m_settings->value("BarMiddleGamma", true).toBool());

    m_settings->endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    storeSettings();
    close();
}

void MainWindow::dialogBluetooth() {
    if(m_dialogBt->isVisible()) {
        m_dialogBt->hide();
    } else {
        m_dialogBt->show();
    }
}

void MainWindow::openBluetooth() {
    if (m_bluetooth->isOpen()) {
        disconnectBluetooth();
    } else {
        m_connection = ConnectBluetooth;
        m_dialogSession->show();
    }
}

void MainWindow::BtServicesDiscovering() {
    emit logMessage(QString(tr("Services Discovering %2 [%1]"))
                            .arg(m_dialogBt->deviceAddress())
                            .arg(m_dialogBt->deviceName()));
    m_optionsMenu->setDisabled(true);
}

void MainWindow::BtServicesError(const BTServices::BTServicesError &error) {
    emit logMessage(QString(tr("Services %2 [%1] Discovering Error: %3"))
                            .arg(m_dialogBt->deviceAddress())
                            .arg(m_dialogBt->deviceName())
                            .arg(error));
    m_optionsMenu->setEnabled(true);
}

void MainWindow::BtServiceDiscovered(const QBluetoothServiceInfo & service) {
    emit logMessage(QString(tr("Service %1 of %2/%3 Discovered"))
                            .arg(service.serviceUuid().toString())
                            .arg(m_dialogBt->deviceAddress())
                            .arg(m_dialogBt->deviceName()));
}

void MainWindow::BtServicesDiscovered(const QBluetoothServiceInfo & sppservice) {
    // qDebug() << "Services Discovered" << sppservice;
    emit logMessage(QString(tr("Services %2 [%1] Discovered"))
                            .arg(m_dialogBt->deviceAddress())
                            .arg(m_dialogBt->deviceName()));

}

void MainWindow::BtSocketError(QBluetoothSocket::SocketError error) {
    emit logMessage(QString(tr("Bt %2 [%1] Socket Error: %3"))
        .arg(m_dialogBt->deviceAddress())
        .arg(m_dialogBt->deviceName())
        .arg(static_cast<int>(error)));
    m_optionsMenu->setEnabled(true);
    m_connectBt->setText(tr("Connect BT"));
    m_connectGen->setEnabled(true);
}

void MainWindow::BtSocketConnecting() {
    emit logMessage(QString(tr("Bt %2 [%1] Socket Connecting"))
                            .arg(m_dialogBt->deviceAddress())
                            .arg(m_dialogBt->deviceName()));
    m_connectBt->setDisabled(true);
    m_connectBt->setText(tr("Connecting BT"));
    m_connectGen->setDisabled(true);
}

void MainWindow::BtSocketConnected() {
    emit logMessage(QString(tr("Bt %2 [%1] Socket Connected"))
                            .arg(m_dialogBt->deviceAddress())
                            .arg(m_dialogBt->deviceName()));
    m_optionsMenu->setEnabled(true);
    m_connectBt->setEnabled(true);
    m_connectBt->setText(tr("Disconnect BT"));
    m_connectGen->setDisabled(true);
    m_eegWave->getWidget()->start();
}

void MainWindow::BtSocketDisconnected() {
    m_optionsMenu->setEnabled(true);
    m_connectBt->setEnabled(true);
    m_connectBt->setText(tr("Connect BT"));
    m_connectGen->setEnabled(true);
    emit logMessage(QString(tr("Bt %2 [%1] Socket Disconnected"))
                            .arg(m_dialogBt->deviceAddress())
                            .arg(m_dialogBt->deviceName()));
    m_eegWave->getWidget()->stop();
}

void MainWindow::logMessage(const QString &message) {
    statusBar()->showMessage(message);
    emit m_eegLog->appendMessage("[" + QDateTime::currentDateTime().toString() + "] " + message);
}

void MainWindow::openGenerator(void) {
    if (m_generator->isEnabled()) {
        disconnectGenerator();
    } else {
        m_connection = ConnectGenerator;
        m_dialogSession->show();
    }
}

void MainWindow::connectGenerator() {
    m_parser->setSessionId(m_dialogSession->getSession().getId());
    emit m_eegSessions->getWidget()->updateSessionTable();
    m_generator->startReceive();
    emit m_eegWave->getWidget()->start();
    m_connectGen->setText(tr("Disconnect Generator"));
    m_connectBt->setDisabled(true);
}

void MainWindow::connectBluetooth() {
    emit logMessage(QString(tr("Connect BT %2 [%1]"))
                            .arg(m_dialogBt->deviceAddress())
                            .arg(m_dialogBt->deviceName()));
    emit m_eegSessions->getWidget()->updateSessionTable();
    m_parser->setSessionId(m_dialogSession->getSession().getId());
    m_bluetooth->startReceive();
}

void MainWindow::connectSerailPort() {

}

void MainWindow::disconnectGenerator() {
    m_generator->stopReceive();
    emit m_eegWave->getWidget()->stop();
    m_connectGen->setText(tr("Connect Generator"));
    m_connectBt->setEnabled(true);
}

void MainWindow::disconnectBluetooth() {
    m_bluetooth->stopReceive();
}

void MainWindow::disconnectSerialPort() {

}

void MainWindow::acceptSession(void) {
    switch(m_connection) {
        case ConnectGenerator:
            connectGenerator();
            break;
        case ConnectBluetooth:
            connectBluetooth();
            break;
        case ConnectSerialPort:
            connectSerailPort();
            break;
        default:
            break;
    }
}
