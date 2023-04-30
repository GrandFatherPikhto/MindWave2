//
// Created by grand on 10.03.2023.
//
#pragma once

#include <QDebug>
#include <QWidget>
#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QChartView>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include <vector>
#include <list>
#include <queue>

#include "../../TGParser/EEGData.h"

using namespace std;

QT_FORWARD_DECLARE_CLASS(TGParser);
QT_FORWARD_DECLARE_CLASS(QTimer);

class EEGBar : public QChartView
{
Q_OBJECT
public:
    explicit EEGBar(QWidget *parent = nullptr);

    ~EEGBar();

public slots:
    void setEegAsicPower(EEGData &);
    void reRead();

    void enableDelta(bool enable);
    void enableTheta(bool enable);
    void enableLowAlpha(bool enable);
    void enableHighAlpha(bool enable);
    void enableLowBeta(bool enable);
    void enableHighBeta(bool enable);
    void enableLowGamma(bool enable);
    void enableMiddleGamma(bool enable);


signals:

private:
    void createBarsets();
    void createSeries();

    QChart *m_chart;
    QList<QBarSet*> m_barsets;
    // vector<QBarSet *> m_barsets;
    // vector<QBarSeries *> m_waveSeries;
    QBarSeries *m_series;
    QList<QBarSeries *> m_barseries;
    // vector<QString> m_waves;
    QStringList m_waves;
    TGParser *m_parser;
    QTimer *m_timer;
    queue<EEGData> m_queue;
};

