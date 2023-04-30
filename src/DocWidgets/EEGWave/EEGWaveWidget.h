//
// Created by grand on 11.03.2023.
//
#pragma once

#include <QDockWidget>
#include "../../TGParser/EEGData.h"
#include <vector>

using namespace std;

QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(EEGWave)
QT_FORWARD_DECLARE_CLASS(TGParser)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSettings)

class EEGWaveWidget : public QDockWidget {
    Q_OBJECT
public:
    explicit EEGWaveWidget(const QString &name, const QString &windowTitle, QWidget *parent = nullptr);
    ~EEGWaveWidget() override;

    inline EEGWave * getWidget() { return m_eegwave; }

    void closeEvent(QCloseEvent *event) override;

public slots:


signals:
    void setPoorSignal(uint8_t poor);
    void setEegAsicPower(EEGData &);

private slots:
    void fillIntervalLabel(void);
    void fillLengthLabel(void);

private:
    void initLayout(void);
    void initLabels(void);
    void initSliders(void);
    void initConnections(void);
    void storeSettings(void);
    void restoreSettings(void);

    QWidget     *m_container;
    QGridLayout *m_layout;

    EEGWave     *m_eegwave;
    TGParser    *m_parser;
    QSlider     *m_intervalSlider;
    QSlider     *m_lengthSlider;
    QSlider     *m_topSlider;
    QSlider     *m_botSlider;
    QSettings   *m_settings;
    QLabel      *m_intervalLabel;
    QLabel      *m_lengthLabel;
};


