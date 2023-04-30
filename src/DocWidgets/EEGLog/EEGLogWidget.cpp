//
// Created by denis on 18.04.2023.
//

#include "EEGLogWidget.h"
#include "EEGLog.h"
#include "../../TGParser/TGParser.h"

#include <QDockWidget>
#include <QGridLayout>
#include <QWidget>

EEGLogWidget::EEGLogWidget(const QString &name, const QString &windowTitle, QWidget *parent) :
    QDockWidget(windowTitle, parent)
    , m_container(new QWidget(this))
    , m_layout(new QGridLayout(m_container))
    , m_eeglog(new EEGLog(m_container))
    , m_parser(nullptr)
{
    initLayout();
    initConnections();
}

EEGLogWidget::~EEGLogWidget () {

}

void EEGLogWidget::initLayout(void) {
    m_layout->addWidget(m_eeglog);
    m_container->setLayout(m_layout);
    setWidget(m_container);
}

void EEGLogWidget::initConnections(void) {
    connect(this, SIGNAL(appendMessage(const QString &)), m_eeglog, SLOT(appendMessage(const QString &)));
}