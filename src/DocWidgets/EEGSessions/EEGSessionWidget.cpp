//
// Created by denis on 28.04.2023.
//

#include "EEGSessionWidget.h"
#include "EEGSession.h"

#include <QGridLayout>
#include <QSettings>
#include <QSlider>
#include <QPixmap>
#include <QLabel>

#define SAVE_GROUP "EEGSessionWidget"

EEGSessionWidget::EEGSessionWidget(const QString &name, const QString &windowTitle, QWidget *parent) :
    QDockWidget(windowTitle, parent)
    , m_session(new EEGSession(this))
    , m_container(new QWidget(this))
    , m_layout(new QGridLayout(m_container))
    , m_settings(new QSettings(tr("EScripka"), tr("MindWave2")))
{
    initLayout();
    restoreSettings();
}

EEGSessionWidget::~EEGSessionWidget() {
    storeSettings();
}

void EEGSessionWidget::initLayout() {
    m_layout->addWidget(m_session);
    m_container->setLayout(m_layout);
    setWidget(m_container);
}

void EEGSessionWidget::storeSettings() {

}

void EEGSessionWidget::restoreSettings() {

}


