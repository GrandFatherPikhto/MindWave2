//
// Created by denis on 18.04.2023.
//
#pragma once

#ifndef MINDWAVE2_EEGSTATUSWIDGET_H
#define MINDWAVE2_EEGSTATUSWIDGET_H

#include <QDebug>
#include <QDockWidget>

QT_FORWARD_DECLARE_CLASS(QGridLayout);
QT_FORWARD_DECLARE_CLASS(EEGStatus);

class EEGStatusWidget : public QDockWidget {
    Q_OBJECT
public:
    explicit EEGStatusWidget(const QString &name, const QString &windowTitle, QWidget *parent = nullptr);
    ~EEGStatusWidget() override;

    inline EEGStatus * getWidget() { return m_eegstatus; }

signals:
    void setAttention(uint8_t);
    void setMeditation(uint8_t);
    void setPoorSignal(uint8_t);
    void setBlinkStrength(uint8_t);

private:
    void initLayout(void);
    void initConnections(void);

    QWidget     *m_container;
    QGridLayout *m_layout;
    EEGStatus   *m_eegstatus;
};


#endif //MINDWAVE2_EEGSTATUSWIDGET_H
