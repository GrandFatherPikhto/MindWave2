//
// Created by grand on 10.03.2023.
//
#include <QDockWidget>
#include <vector>

#include "../../TGParser/EEGData.h"

#pragma once

using namespace std;

QT_FORWARD_DECLARE_CLASS(EEGBar);
QT_FORWARD_DECLARE_CLASS(QGridLayout);
QT_FORWARD_DECLARE_CLASS(QWidget);
QT_FORWARD_DECLARE_CLASS(TGParser);


class EEGBarWidget : public QDockWidget {
    Q_OBJECT
public:
    explicit EEGBarWidget(const QString &name, const QString &windowTitle, QWidget *parent = nullptr);
    ~EEGBarWidget() override;

    inline EEGBar * getWidget() { return m_eegbar; }

signals:
    void setEegAsicPower(EEGData &);

public slots:
    // void setParser(TGParser *parser);

private:
    void initLayout(void);
    void initConnections(void);

    QGridLayout     *m_layout;
    QWidget         *m_container;
    EEGBar          *m_eegbar;
    const TGParser  *m_parser;
};

