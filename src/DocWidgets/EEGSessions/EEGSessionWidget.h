//
// Created by denis on 28.04.2023.
//
#pragma once

#include <QDockWidget>

QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(EEGWave)
QT_FORWARD_DECLARE_CLASS(TGParser)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSettings)
QT_FORWARD_DECLARE_CLASS(EEGSession)
QT_FORWARD_DECLARE_CLASS(QShowEvent)

class EEGSessionWidget : public QDockWidget {
    Q_OBJECT
public:
    explicit EEGSessionWidget(const QString &name, const QString &windowTitle, QWidget *parent = nullptr);
    ~EEGSessionWidget() override;

    inline EEGSession * getWidget() { return m_session; }

public slots:

private:
    void initLayout      ();
    void storeSettings   ();
    void restoreSettings ();

    EEGSession  *m_session;
    QWidget     *m_container;
    QGridLayout *m_layout;
    QSettings   *m_settings;
};

