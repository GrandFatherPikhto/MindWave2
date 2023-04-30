#ifndef EEGSTATE_H
#define EEGSTATE_H

#include <QDebug>
#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QBarCategoryAxis>
#include <QBarSet>
#include <QBarSeries>
#include <QValueAxis>
#include <QTimer>
#include "../../TGParser/EEGData.h"

#include <vector>
#include <queue>

using namespace std;

class EEGStatus : public QChartView
{
    Q_OBJECT
public:
    explicit EEGStatus(QWidget *parent = nullptr);
    ~EEGStatus();

signals:

public slots:
    void setAttention(uint8_t);
    void setMeditation(uint8_t);
    void setPoorSignal(uint8_t);
    void setBlinkStrength(uint8_t);
    void reRead(void);

private:
    QChart     *m_chart;
    QBarSeries *m_attention;
    QBarSeries *m_meditaion;
    queue<::uint8_t> m_queueAttention;
    queue<::uint8_t> m_queueMeditation;

    QTimer     *m_timer;
    // QBarSeries *m_poorsignal;
};

#endif // EEGSTATE_H
