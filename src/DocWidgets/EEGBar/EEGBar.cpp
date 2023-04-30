//
// Created by grand on 10.03.2023.
//
#include <QTimer>

#include "EEGBar.h"
#include "../../Connectors/Generator/Generator.h"
//#include "../TGParser/TGParser.h"
//#include "../TGParser/EEGData.h"

#define MAX_QUEUE_SIZE 0x100

EEGBar::EEGBar(QWidget *parent) :
        QChartView(parent)
        , m_chart(new QChart)
        // , m_barsets(vector<QBarSet *>(EEGPOINTS))
        // , m_waveSeries(vector<QBarSeries *>(EEGPOINTS))
        , m_series(new QBarSeries)
        , m_timer(new QTimer(this))
{
    m_waves
            << tr("Delta") << tr("Thetta") << tr("Low Alpha") << tr("High Alpha") << tr("Low Beta")
            << tr("High Beta") << tr("Low Gamma") << tr("Mid Gamma");
    // createBarsets();

    createSeries();
    this->setChart(m_chart);
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(reRead()));
    m_timer->start(2000);
}

EEGBar::~EEGBar() {

}

void EEGBar::createBarsets() {
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 1.0);
    m_barsets
            << new QBarSet(tr("Delta"))
            << new QBarSet(tr("Thetta"))
            << new QBarSet(tr("Low Alpha"))
            << new QBarSet(tr("High Alpha"))
            << new QBarSet(tr("Low Beta"))
            << new QBarSet(tr("High Beta"))
            << new QBarSet(tr("Low Gamma"))
            << new QBarSet(tr("Mid Gamma"));
    m_series->append(m_barsets);
    *m_barsets.at(0) << .3;
    *m_barsets.at(1) << .3;
    *m_barsets.at(2) << .3;
    *m_barsets.at(3) << .3;
    *m_barsets.at(4) << .3;
    *m_barsets.at(5) << .3;
    *m_barsets.at(6) << .3;
    *m_barsets.at(7) << .3;
    m_chart->addSeries(m_series);
    m_series->setBarWidth(.9);

    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_series->attachAxis(axisY);

    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    this->setRenderHint(QPainter::Antialiasing);
}

/**
 * @brief EEGBar::createSeries
 */
void EEGBar::createSeries() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 100);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 1.0);
    m_chart->addAxis(axisY, Qt::AlignLeft);

    QBarCategoryAxis *axisX = new QBarCategoryAxis;
    axisX->append(m_waves);
    m_chart->addAxis(axisX, Qt::AlignBottom);

    for(uint8_t i = 0; i < EEGPOINTS; i++) {
        m_barsets.append(new QBarSet(m_waves.at(i)));
        double value = distrib(gen)/100.0;
        *m_barsets.at(i) << value;
        m_barseries.append(new QBarSeries);
        m_barseries.at(i)->append(m_barsets.at(i));
        m_barseries.at(i)->setBarWidth(.9);
        m_chart->addSeries(m_barseries.at(i));
        m_barseries.at(i)->attachAxis(axisY);
    }
}

void EEGBar::setEegAsicPower(EEGData & eegData) {
    if (eegData.isDoubleDataValid()) {
        m_queue.push(eegData);
        if (m_queue.size() > MAX_QUEUE_SIZE) {
            m_queue.pop();
        }
    }
}

void EEGBar::reRead() {
    if (!m_queue.empty()) {
        EEGData eegData = m_queue.front();

        if (eegData.isDoubleDataValid()) {
            m_barseries.at(EEGData::Delta)->barSets().at(0)->replace(0, eegData.delta());
            m_barseries.at(EEGData::Theta)->barSets().at(0)->replace(0, eegData.theta());
            m_barseries.at(EEGData::LowAlpha)->barSets().at(0)->replace(0, eegData.lowAlpha());
            m_barseries.at(EEGData::HighAlpha)->barSets().at(0)->replace(0, eegData.highAlpha());
            m_barseries.at(EEGData::LowBeta)->barSets().at(0)->replace(0, eegData.lowBeta());
            m_barseries.at(EEGData::HighBeta)->barSets().at(0)->replace(0, eegData.highBeta());
            m_barseries.at(EEGData::LowGamma)->barSets().at(0)->replace(0, eegData.lowGamma());
            m_barseries.at(EEGData::MiddleGamma)->barSets().at(0)->replace(0, eegData.midGamma());
        }

        while (!m_queue.empty()) {
            m_queue.pop();
        }
    }
}

void EEGBar::enableDelta(bool enable) {
    m_barseries.at(EEGData::Delta)->setVisible(enable);
}

void EEGBar::enableTheta(bool enable) {
    m_barseries.at(EEGData::Theta)->setVisible(enable);
}

void EEGBar::enableLowAlpha(bool enable) {
    m_barseries.at(EEGData::LowAlpha)->setVisible(enable);
}

void EEGBar::enableHighAlpha(bool enable) {
    m_barseries.at(EEGData::HighAlpha)->setVisible(enable);
}

void EEGBar::enableLowBeta(bool enable) {
    m_barseries.at(EEGData::LowBeta)->setVisible(enable);
}

void EEGBar::enableHighBeta(bool enable) {
    m_barseries.at(EEGData::HighBeta)->setVisible(enable);
}

void EEGBar::enableLowGamma(bool enable) {
    m_barseries.at(EEGData::LowGamma)->setVisible(enable);
}

void EEGBar::enableMiddleGamma(bool enable) {
    m_barseries.at(EEGData::MiddleGamma)->setVisible(enable);
}



