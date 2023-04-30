#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>

#include <random>
#include <vector>
#include <string>

//#include <sys/time.h>

#include "../../TGParser/TGParser.h"

#define EEGPOINTS 8
#define EEGTIMEINTEVAL 1000
#define POSTUREINTERVAL 5000
#define PACKETLEN 0xFF

using namespace std;

class Generator : public QObject
{
    Q_OBJECT
public:
    explicit Generator(TGParser *parser, QObject *parent = nullptr);
    ~Generator();

    void timerEvent(QTimerEvent *) override;
    inline bool isEnabled() { return m_enable; }

signals:
    void generateStart();
    void generateFinish();

public slots:
    void startReceive();
    void stopReceive();

private:
    void generate();
    void generateAsicEeg();
    void resetPacket();
    void generatePacketlen();
    void generatePoorSignal();
    void generateAttention();
    void generateMeditation();
    void generateChecksum();
    void generateOld();
    void printPacket();
    void sendPacket();

    TGParser *m_parser;
    bool m_enable;
    // struct timeval m_tv;
    uint64_t m_eegtime;
    uint8_t m_packet[PACKETLEN];
    uint8_t m_pos;

    vector<uint32_t> m_eeg;
    uint16_t m_plen;
    uint8_t m_poorsignal;
    uint8_t m_attention;
    uint8_t m_meditation;
    uint8_t m_checksum;
};

#endif // GENERATOR_H
