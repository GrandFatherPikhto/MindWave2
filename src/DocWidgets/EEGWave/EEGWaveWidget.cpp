//
// Created by grand on 11.03.2023.
//

#include "EEGWaveWidget.h"
#include "EEGWave.h"
#include "../../TGParser/TGParser.h"

#include <QGridLayout>
#include <QSettings>
#include <QSlider>
#include <QPixmap>
#include <QMatrix4x4>
#include <QLabel>

#define SAVE_GROUP    "EEGWaveWidget"
#define SAVE_INTERVAL "interval"
#define SAVE_LENGTH   "points"
#define SAVE_TOP      "top"
#define SAVE_BOT      "bottom"

EEGWaveWidget::EEGWaveWidget(const QString &name, const QString &windowTitle, QWidget* pParent) :
        QDockWidget(windowTitle, pParent)
        , m_container(new QWidget(this))
        , m_eegwave(new EEGWave(m_container))
        , m_intervalLabel(new QLabel(m_container))
        , m_lengthLabel(new QLabel(m_container))
        , m_intervalSlider(new QSlider(Qt::Horizontal, m_container))
        , m_lengthSlider(new QSlider(Qt::Horizontal, m_container))
        , m_topSlider(new QSlider(Qt::Vertical, m_container))
        , m_botSlider(new QSlider(Qt::Vertical, m_container))
        , m_layout(new QGridLayout(m_container))
        , m_settings(new QSettings(tr("EScripka"), tr("MindWave2")))
{
    initLayout();
    initConnections();
    restoreSettings();
}

EEGWaveWidget::~EEGWaveWidget() noexcept {
    storeSettings();
}

void EEGWaveWidget::initSliders(void) {
    m_intervalSlider->setTickPosition(QSlider::TicksBelow);
    m_intervalSlider->setTickInterval(500);
    m_intervalSlider->setMinimum(1000);
    m_intervalSlider->setMaximum(11000);
    m_lengthSlider->setTickInterval(10);
    m_lengthSlider->setTickPosition(QSlider::TicksBelow);
    m_lengthSlider->setMinimum(2);
    m_lengthSlider->setMaximum(100);
    m_topSlider->setMinimum(0);
    m_topSlider->setMaximum(150);
    m_botSlider->setMinimum(-50);
    m_botSlider->setMaximum(0);
}


void EEGWaveWidget::initLayout() {
    initSliders();
    initLabels();
    m_layout->addWidget(m_intervalLabel, 0, 0, 1, 2);
    m_layout->addWidget(m_intervalSlider, 1, 0, 1, 2);
    m_layout->addWidget(m_topSlider, 2, 0);
    m_layout->addWidget(m_botSlider, 3, 0);
    m_layout->addWidget(m_eegwave, 2, 1, 2, 1);
    m_layout->addWidget(m_lengthLabel, 4, 1, 1, 1);
    m_layout->addWidget(m_lengthSlider, 5, 1, 1, 1);
    m_container->setLayout(m_layout);
    setWidget(m_container);
}

void EEGWaveWidget::initConnections() {
    connect(this, SIGNAL(setPoorSignal(uint8_t)),
            m_eegwave, SLOT(setPoorSignal(uint8_t)));
    connect(this, SIGNAL(setEegAsicPower(EEGData &)),
            m_eegwave, SLOT(setEegAsicPower(EEGData &)));
    connect(m_intervalSlider, SIGNAL(valueChanged(int)),
            m_eegwave, SLOT(setInterval(int)));
    connect(m_lengthSlider, SIGNAL(valueChanged(int)),
            m_eegwave, SLOT(setLength(int)));
    connect(m_topSlider, SIGNAL(valueChanged(int)), m_eegwave, SLOT(setTop(int)));
    connect(m_botSlider, SIGNAL(valueChanged(int)), m_eegwave, SLOT(setBottom(int)));

    connect(m_intervalSlider, SIGNAL(valueChanged(int)), this, SLOT(fillIntervalLabel()));
    connect(m_lengthSlider, SIGNAL(valueChanged(int)), this, SLOT(fillLengthLabel()));
}

void EEGWaveWidget::storeSettings(void) {
    m_settings->beginGroup(SAVE_GROUP);
    m_settings->setValue(SAVE_INTERVAL, m_intervalSlider->value());
    m_settings->setValue(SAVE_LENGTH, m_lengthSlider->value());
    m_settings->setValue(SAVE_TOP, m_topSlider->value());
    m_settings->setValue(SAVE_BOT, m_botSlider->value());
    m_settings->endGroup();
}

void EEGWaveWidget::restoreSettings(void) {
    m_settings->beginGroup(SAVE_GROUP);
    m_intervalSlider->setValue(m_settings->value(SAVE_INTERVAL, 1000).toInt());
    m_lengthSlider->setValue(m_settings->value(SAVE_LENGTH, 30).toInt());
    m_topSlider->setValue(m_settings->value(SAVE_TOP, 130).toInt());
    m_botSlider->setValue(m_settings->value(SAVE_BOT, -30).toInt());
    m_settings->endGroup();
}

void EEGWaveWidget::closeEvent(QCloseEvent *event) {
    storeSettings();
    QDockWidget::closeEvent(event);
}

void EEGWaveWidget::fillIntervalLabel(void) {
    m_intervalLabel->setText(QString(tr("Interval: %1s")).arg(static_cast<double>(m_intervalSlider->value())/1000.0));
}

void EEGWaveWidget::fillLengthLabel(void) {
    m_lengthLabel->setText(QString(tr("Length: %1 points")).arg(m_lengthSlider->value()));
}

void EEGWaveWidget::initLabels(void) {
//    QTransform t;
//    t.rotate(45, Qt::XAxis);
//    QPixmap pixmap(m_intervalLabel->pixmap());
//    pixmap.transformed(t);
//    m_intervalLabel->setPixmap(pixmap);
    // m_intervalLabel->setText(tr("Top"));
}

