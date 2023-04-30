//
// Created by grand on 11.03.2023.
//

#include "EEGWave.h"
#include "../../TGParser/EEGData.h"
#include "../../TGParser/TGParser.h"

#include <QValueAxis>
#include <QSplineSeries>
#include <QLineSeries>
#include <QSettings>
#include <QTimer>

#define INTERVAL "EEGWave/interval"

EEGWave::EEGWave(QWidget *parent) :
        QChartView(parent)
        , m_chart(new QChart)
        , m_axisX(new QValueAxis)
        , m_axisY(new QValueAxis)
        , m_length(20)
        , m_timer(new QTimer(this))
        , m_parser(nullptr)
        , m_interval(0)
        , m_settings(new QSettings(tr("EScripka"), tr("MindWave2")))
{
    QStringList names;
    names << tr("Delta") << tr("Theta") << tr("Low Alpha")
        << tr("High Alpha") << tr("Low Beta")
        << tr("High Beta") << tr("Low Gamm") << tr("Mid Gamma");

    // m_axisX->setRange(0, size);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_axisX->setRange(0.0, static_cast<double>(m_length));
    m_axisY->setRange(-0.2, 1.2);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_chart->setTitle(tr("Linear EEG"));

    for(uint8_t i = 0; i < NUMWAVES; i++) {
        m_waveSeries.append(new QSplineSeries);
        m_chart->addSeries(m_waveSeries.at(i));
        m_waveSeries.at(i)->attachAxis(m_axisX);
        m_waveSeries.at(i)->attachAxis(m_axisY);
        m_waveSeries.at(i)->setName(names.at(i));
    }

    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    this->setChart(m_chart);
    this->setRenderHint(QPainter::Antialiasing);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(reRead()));
    restoreSettings();
    initQueues();
}

EEGWave::~EEGWave() {
    storeSettings();
}

void EEGWave::setEegAsicPower(EEGData & eegData) {
    if (eegData.isDoubleDataValid()) {
        appendQueue(m_delta, eegData.delta());
        appendQueue(m_theta, eegData.theta());
        appendQueue(m_lowAlpha, eegData.lowAlpha());
        appendQueue(m_highAlpha, eegData.highAlpha());
        appendQueue(m_lowBeta, eegData.lowBeta());
        appendQueue(m_highBeta, eegData.highBeta());
        appendQueue(m_lowGamma, eegData.lowGamma());
        appendQueue(m_midGamma, eegData.midGamma());
    }
}

void EEGWave::setPoorSignal(uint8_t poor) {
    qDebug() << "Poor Signal " << poor;
}

void EEGWave::reRead() {
    m_waveSeries.at(EEGData::Delta)->replace(m_delta);
    m_waveSeries.at(EEGData::Theta)->replace(m_theta);
    m_waveSeries.at(EEGData::LowAlpha)->replace(m_lowAlpha);
    m_waveSeries.at(EEGData::HighAlpha)->replace(m_highAlpha);
    m_waveSeries.at(EEGData::LowBeta)->replace(m_lowBeta);
    m_waveSeries.at(EEGData::HighBeta)->replace(m_highBeta);
    m_waveSeries.at(EEGData::LowGamma)->replace(m_lowGamma);
    m_waveSeries.at(EEGData::MiddleGamma)->replace(m_midGamma);
}

void EEGWave::appendQueue(QList<QPointF> &wave, double point) {
    for (int idx = 0; idx < wave.size() - 1; idx ++) {
        wave[idx].setY(wave[idx + 1].y());
    }
    wave[wave.size() - 1].setY(point);
}

void EEGWave::setLength(int size) {
    m_length = size;
    initQueues();
}

void EEGWave::initQueues(void) {
    m_axisX->setRange(0.0, static_cast<double>(m_length - 1));
    m_delta.clear();
    m_theta.clear();
    m_lowAlpha.clear();
    m_highAlpha.clear();
    m_lowBeta.clear();
    m_highBeta.clear();
    m_lowGamma.clear();
    m_midGamma.clear();
    for (int i = 0; i <= m_length; i++) {
        m_delta.append(QPointF(static_cast<double>(i), 0.0));
        m_theta.append(QPointF(static_cast<double>(i), 0.0));
        m_lowAlpha.append(QPointF(static_cast<double>(i), 0.0));
        m_highAlpha.append(QPointF(static_cast<double>(i), 0.0));
        m_lowBeta.append(QPointF(static_cast<double>(i), 0.0));
        m_highBeta.append(QPointF(static_cast<double>(i), 0.0));
        m_lowGamma.append(QPointF(static_cast<double>(i), 0.0));
        m_midGamma.append(QPointF(static_cast<double>(i), 0.0));
    }
}

void EEGWave::start() {
    m_timer->start(m_interval);
}

void EEGWave::stop() {
    m_timer->stop();
}

void EEGWave::setInterval(int interval) {
    m_interval = interval;
    if (m_timer->isActive()) {
        m_timer->stop();
        m_timer->start(m_interval);
    }
}

void EEGWave::storeSettings(void) {
    m_settings->setValue(INTERVAL, m_interval);
}

void EEGWave::restoreSettings(void) {
    int interval = m_settings->value(INTERVAL, 1000).toInt();
    setInterval(interval);
}

void EEGWave::setTop(int top) {
     m_axisY->setRange(m_axisY->min(), static_cast<float>(top) / 100.0);
}

void EEGWave::setBottom(int bottom) {
    m_axisY->setRange(static_cast<float>(bottom) / 100.0, m_axisY->max());
}

void EEGWave::enableDelta(bool enable) {
    m_waveSeries.at(EEGData::Delta)->setVisible(enable);
}

void EEGWave::enableTheta(bool enable) {
    m_waveSeries.at(EEGData::Theta)->setVisible(enable);
}

void EEGWave::enableLowAlpha(bool enable) {
    m_waveSeries.at(EEGData::LowAlpha)->setVisible(enable);
}

void EEGWave::enableHighAlpha(bool enable) {
    m_waveSeries.at(EEGData::HighAlpha)->setVisible(enable);
}

void EEGWave::enableLowBeta(bool enable) {
    m_waveSeries.at(EEGData::LowBeta)->setVisible(enable);
}

void EEGWave::enableHighBeta(bool enable) {
    m_waveSeries.at(EEGData::HighBeta)->setVisible(enable);
}

void EEGWave::enableLowGamma(bool enable) {
    m_waveSeries.at(EEGData::LowGamma)->setVisible(enable);
}

void EEGWave::enableMiddleGamma(bool enable) {
    m_waveSeries.at(EEGData::MiddleGamma)->setVisible(enable);
}

void EEGWave::clean() {
    initQueues();
    emit reRead();
}
