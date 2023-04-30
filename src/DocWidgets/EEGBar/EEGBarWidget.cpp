//
// Created by grand on 10.03.2023.
//

#include "EEGBarWidget.h"
#include "EEGBar.h"
#include "../../TGParser/TGParser.h"

#include <QGridLayout>
#include <QWidget>

EEGBarWidget::EEGBarWidget(const QString &name, const QString &windowTitle, QWidget* pParent) :
    QDockWidget(windowTitle, pParent)
    , m_container(nullptr)
    , m_layout(nullptr)
    , m_eegbar(nullptr)
    , m_parser(nullptr)
{
    initLayout();
    initConnections();
}

EEGBarWidget::~EEGBarWidget() noexcept {

}

void EEGBarWidget::initLayout(void) {
    m_container = new QWidget(this);
    m_layout    = new QGridLayout(m_container);

    m_eegbar    = new EEGBar(m_container);
    m_layout->addWidget(m_eegbar);

    m_container -> setLayout(m_layout);

    setWidget(m_container);
}

void EEGBarWidget::initConnections() {
    connect(this, SIGNAL(setEegAsicPower(EEGData &)), m_eegbar, SLOT(setEegAsicPower(EEGData &)));
}
