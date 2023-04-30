#ifndef DialogSP_H
#define DialogSP_H

#include <QDebug>
#include <QDialog>
#include <QSerialPort>
#include <QIntValidator>
#include <QSettings>

class DialogSP : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSP(QWidget *parent = nullptr);
    ~DialogSP();

public:
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    Settings settings() const;

private slots:
    void showPortInfo(int idx);
    void apply();
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();
    void saveSettings();
    void loadSettings();

private:
    Settings m_currentSettings;
    QIntValidator *m_intValidator = nullptr;

};

#endif // DialogSP_H
