//
// Created by denis on 18.04.2023.
//
#include <QGridLayout>
#include "EEGStatus.h"
#include "EEGStatusWidget.h"

EEGStatusWidget::EEGStatusWidget(const QString &name, const QString &windowTitle, QWidget *parent) :
        QDockWidget(windowTitle, parent)
        , m_container(new QWidget(this))
        , m_layout(new QGridLayout(m_container))
        , m_eegstatus(new EEGStatus(m_container))
{
    initLayout();
    initConnections();
}

EEGStatusWidget::~EEGStatusWidget () {

}

void EEGStatusWidget::initLayout(void) {
    m_layout->addWidget(m_eegstatus);
    m_container->setLayout(m_layout);
    setWidget(m_container);
}

void EEGStatusWidget::initConnections(void) {
    connect(this, SIGNAL(setPoorSignal(uint8_t)), m_eegstatus, SLOT(setPoorSignal(uint8_t)));
    connect(this, SIGNAL(setAttention(uint8_t)), m_eegstatus, SLOT(setAttention(uint8_t)));
    connect(this, SIGNAL(setMeditation(uint8_t)), m_eegstatus, SLOT(setMeditation(uint8_t)));
    connect(this, SIGNAL(setBlinkStrength(uint8_t)), m_eegstatus, SLOT(setBlinkStrength(uint8_t)));
}