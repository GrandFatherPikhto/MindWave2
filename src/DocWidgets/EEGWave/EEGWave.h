//
// Created by grand on 11.03.2023.
//
#pragma once

#include <QChartView>
#include <vector>
#include <queue>

#include "../../TGParser/EEGData.h"

using namespace std;

QT_FORWARD_DECLARE_CLASS(QValueAxis)
QT_FORWARD_DECLARE_CLASS(QSplineSeries)
QT_FORWARD_DECLARE_CLASS(QLineSeries)
QT_FORWARD_DECLARE_CLASS(TGParser)
QT_FORWARD_DECLARE_CLASS(QSettings)

#define NUMWAVES 8

class EEGWave : public QChartView {
    Q_OBJECT
public:
    explicit EEGWave(QWidget *parent = nullptr);
    ~EEGWave();

signals:

public slots:
    void setPoorSignal(uint8_t poor);
    void setEegAsicPower(EEGData &);
    void reRead();
    void setLength(int lenght);
    void start();
    void stop();
    void clean();
    void setInterval(int interval);
    void setTop(int top);
    void setBottom(int bottom);
    void enableDelta(bool enable);
    void enableTheta(bool enable);
    void enableLowAlpha(bool enable);
    void enableHighAlpha(bool enable);
    void enableLowBeta(bool enable);
    void enableHighBeta(bool enable);
    void enableLowGamma(bool enable);
    void enableMiddleGamma(bool enable);

private:
    void appendQueue(QList<QPointF> &wave, double point);
    void initQueues (void);
    void storeSettings(void);
    void restoreSettings(void);

    QChart *m_chart;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QList<QSplineSeries *> m_waveSeries;
    QTimer   *m_timer;
    TGParser *m_parser;
    QList<QPointF> m_delta;
    QList<QPointF> m_theta;
    QList<QPointF> m_lowAlpha;
    QList<QPointF> m_highAlpha;
    QList<QPointF> m_lowBeta;
    QList<QPointF> m_highBeta;
    QList<QPointF> m_lowGamma;
    QList<QPointF> m_midGamma;
    QSettings     *m_settings;
    int m_length;
    int m_interval;
};

