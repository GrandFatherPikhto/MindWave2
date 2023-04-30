//
// Created by denis on 18.04.2023.
//

#ifndef MINDWAVE2_EEGLOG_H
#define MINDWAVE2_EEGLOG_H


#include <QPlainTextEdit>
#include <QFile>

using namespace std;

class EEGLog : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit EEGLog(QWidget *parent = nullptr);
    ~EEGLog() override;

public slots:
    void appendMessage(const QString& text);

private:
    QFile *m_logFile;
    QTextStream *m_logOut;
};


#endif //MINDWAVE2_EEGLOG_H
