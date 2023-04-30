//
// Created by denis on 28.04.2023.
//
#include <QDialogButtonBox>
#include <QWidget>
#include <QGridLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>

#include "DialogSession.h"
#include "../Models/EEGSessionData.h"
#include "../Models/EEGSessionModel.h"

#define SAVE_GROUP "DialogSession"

DialogSession::DialogSession(QWidget *parent) :
    QDialog(parent)
    , m_layout(new QGridLayout(this))
    , m_teName(new QLineEdit(this))
    , m_labelName(new QLabel(tr("Name Session"),this))
    , m_labelDescription(new QLabel(tr("Session Description"), this))
    , m_teDescription(new QTextEdit(this))
    , m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this))
    , m_settings(new QSettings(tr("EScripka"), tr("MindWave2")))
    , m_sessionData(new EEGSessionData())
    , m_sessionModel(new EEGSessionModel(this))
{
    initLayout();
    initConnections();
    restoreSettings();
}

DialogSession::~DialogSession() {
    storeSettings();
}

void DialogSession::initConnections() {
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(acceptSession()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void DialogSession::initLayout() {
    m_layout->addWidget(m_labelName);
    m_layout->addWidget(m_teName);
    m_layout->addWidget(m_labelDescription);
    m_layout->addWidget(m_teDescription);
    m_layout->addWidget(m_buttonBox);
    setLayout(m_layout);
}

void DialogSession::storeSettings() {
    m_settings->beginGroup (SAVE_GROUP);
    m_settings->endGroup   ();
}

void DialogSession::restoreSettings() {
    m_settings->beginGroup (SAVE_GROUP);
    m_settings->endGroup   ();
}

void DialogSession::showEvent(QShowEvent *evt) {
    *m_sessionData = EEGSessionData();
    m_teName->setText(m_sessionData->getName());
    QDialog::showEvent(evt);
}

void DialogSession::acceptSession() {
    if (m_sessionModel->addData(*m_sessionData)) {
        qDebug() << m_sessionData->getId();
        emit accept();
    }
}
