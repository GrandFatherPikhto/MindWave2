#include "SerialPort.h"

/**
 * @brief SerialPort::SerialPort
 * @param parser
 * @param parent
 */
SerialPort::SerialPort(TGParser *parser, QObject *parent) :
    QObject(parent)
  , m_thread(new QThread)
  , m_parser(parser)
  , m_serial(new QSerialPort)
{
    qRegisterMetaType<QSerialPort::SerialPortError>("QSerialPort::SerialPortError");
    moveToThread(m_thread);
    m_thread->start();

    connect(m_serial, &QSerialPort::errorOccurred, this, &SerialPort::handleError);
    connect(m_serial, &QSerialPort::readyRead, this, &SerialPort::readData);
    connect(this, &SerialPort::sendData, this, &SerialPort::writeData);
}

/**
 * @brief SerialPort::~SerialPort
 */
SerialPort::~SerialPort() {

}

/**
 * @brief SerialPort::startReceive
 * @param settings
 */
void SerialPort::startReceive(const DialogSP::Settings &settings) {
    m_serial->setPortName(settings.name);
    m_serial->setBaudRate(settings.baudRate);
    m_serial->setDataBits(settings.dataBits);
    m_serial->setParity(settings.parity);
    m_serial->setStopBits(settings.stopBits);
    m_serial->setFlowControl(settings.flowControl);
    if(m_serial->open(QIODevice::ReadWrite)) {
        emit serialOpen(*m_serial);
        emit statusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                         .arg(m_serial->portName()).arg(m_serial->baudRate()).arg(m_serial->dataBits())
                         .arg(m_serial->parity()).arg(m_serial->stopBits()).arg(m_serial->flowControl()));
    } else {
        emit statusMessage(m_serial->errorString());
    }
}

/**
 * @brief SerialPort::stopReceive
 */
void SerialPort::stopReceive() {
    if (m_serial->isOpen()) {
        m_serial->close();
        emit serialClose(*m_serial);
        emit statusMessage(tr("Disconnect from %1 : %2, %3, %4, %5, %6")
                         .arg(m_serial->portName()).arg(m_serial->baudRate()).arg(m_serial->dataBits())
                         .arg(m_serial->parity()).arg(m_serial->stopBits()).arg(m_serial->flowControl()));
    }
}

/**
 * @brief SerialPort::handleError
 * @param error
 */
void SerialPort::handleError(QSerialPort::SerialPortError error) {
    if(error != QSerialPort::NoError) {
        qDebug() << error;
        emit statusMessage(m_serial->errorString());
        m_serial->clearError();
    }
}

/**
 * @brief SerialPort::writeData
 * @param data
 */
void SerialPort::writeData(const QByteArray &data) {
    Q_UNUSED(data);
}

/**
 * @brief SerialPort::readData
 */
void SerialPort::readData() {
    const QByteArray data = m_serial->readAll();
    if(m_log) {
        // emit statusMessage(data.toHex());
        // qDebug() << data.toHex();
    }
    uint8_t bt;
    foreach(bt, data) {
        m_parser->parseNext(bt);
    }
}

void SerialPort::logData(bool log) {
    m_log = log;
}
