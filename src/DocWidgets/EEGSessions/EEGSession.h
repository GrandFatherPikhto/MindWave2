//
// Created by denis on 28.04.2023.
//

#pragma once

#include <QTableView>

QT_FORWARD_DECLARE_CLASS(EEGSessionModel)
QT_FORWARD_DECLARE_CLASS(QSettings)
QT_FORWARD_DECLARE_CLASS(TimeDelegate)

class EEGSession : public QTableView {
    Q_OBJECT
public:
    explicit EEGSession(QWidget *parent = nullptr);
    ~EEGSession() override;

public slots:
    void updateSessionTable ();

private:
    void storeSettings(void);
    void restoreSettings(void);

    EEGSessionModel *m_model;
    QSettings *m_settings;
    TimeDelegate *m_timeDelegate;
};

