//
// Created by denis on 28.04.2023.
//
#pragma once


#include <QtCore>
#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(QSettings)
QT_FORWARD_DECLARE_CLASS(EEGSessionData)
QT_FORWARD_DECLARE_CLASS(EEGSessionModel)

class DialogSession : public QDialog {
    Q_OBJECT
public:
    explicit DialogSession(QWidget *parent = nullptr);
    ~DialogSession() override;

    void showEvent(QShowEvent *) override;

    inline EEGSessionData &getSession () {
        return *m_sessionData;
    }

private slots:
    void acceptSession   ();

private:
    void initConnections ();
    void initLayout      ();
    void storeSettings   ();
    void restoreSettings ();

    QDialogButtonBox *m_buttonBox;
    QLabel           *m_labelName;
    QLabel           *m_labelDescription;
    QLineEdit        *m_teName;
    QTextEdit        *m_teDescription;
    QGridLayout      *m_layout;
    QSettings        *m_settings;
    QString           m_name;
    QString           m_description;
    int               m_msec;
    EEGSessionData   *m_sessionData;
    EEGSessionModel  *m_sessionModel;
};

