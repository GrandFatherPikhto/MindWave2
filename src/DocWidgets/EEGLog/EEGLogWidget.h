//
// Created by denis on 18.04.2023.
//
#pragma once

#include <QDebug>
#include <QtCore>
#include <QDockWidget>

QT_FORWARD_DECLARE_CLASS(QGridLayout);
QT_FORWARD_DECLARE_CLASS(EEGLog);
QT_FORWARD_DECLARE_CLASS(TGParser);

class EEGLogWidget: public QDockWidget {
    Q_OBJECT
public:
    explicit EEGLogWidget(const QString &name, const QString &windowTitle, QWidget *parent = nullptr);
    ~EEGLogWidget() override;

    inline EEGLog * getWidget() { return m_eeglog; }
signals:
    void appendMessage(const QString &message);

private:
    void initLayout(void);
    void initConnections(void);

    QWidget     *m_container;
    QGridLayout *m_layout;
    EEGLog      *m_eeglog;
    TGParser    *m_parser;
};

