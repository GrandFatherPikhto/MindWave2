#include "Generator.h"

Generator::Generator(TGParser *parser, QObject *parent) :
    QObject(parent)
  , m_parser(parser)
  , m_enable(false)
  , m_eeg(vector<uint32_t>(EEGPOINTS))
{
    startTimer(1000);
}

/**
 * @brief Generator::~Generator
 */
Generator::~Generator() {
    m_enable = false;
}

void Generator::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
    generate();
}

/**
 * @brief Generator::generate
 */
void Generator::generate() {
    m_eegtime = QDateTime::currentMSecsSinceEpoch();
    if(m_enable) {
        resetPacket();
        generatePoorSignal();
        generateAsicEeg();
        generateAttention();
        generateMeditation();
        generateChecksum();
        // printPacket();
        sendPacket();
    }
}

/**
 * @brief Generator::printPacket
 */
void Generator::printPacket() {
    QByteArray out((char*)m_packet, m_pos);
    qDebug() << out.toHex();
}

/**
 * @brief Generator::sendPacket
 */
void Generator::sendPacket() {
    for (uint16_t i = 0; i < m_pos; i++) {
        m_parser->parseNext(m_packet[i]);
    }
}

/**
 * @brief Generator::resetPacket
 */
void Generator::resetPacket() {
    memset(m_packet, 0, PACKETLEN);
    m_packet[0] = 0xAA;
    m_packet[1] = 0xAA;
    m_packet[2] = 0xAA;
    m_packet[3] = 0;
    m_pos = 4;
    generatePacketlen();
}

/**
 * @brief Generator::generatePacketlen
 */
void Generator::generatePacketlen() {
    m_packet[3] = m_pos - 4;
}

/**
 * @brief Generator::generatePoorSignal
 */
void Generator::generatePoorSignal() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(50, 80);
    m_packet[m_pos ++] = 0x02;
    m_poorsignal = distrib(gen);
    m_packet[m_pos ++] = m_poorsignal;
    generatePacketlen();
}

/**
 * @brief Generator::generateAttention
 */
void Generator::generateAttention() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 100);
    m_packet[m_pos ++] = 0x04;
    m_attention        = distrib(gen);
    m_packet[m_pos ++] = m_attention;
    generatePacketlen();
}

/**
 * @brief Generator::generateMeditation
 */
void Generator::generateMeditation() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 100);
    m_packet[m_pos ++] = 0x05;
    m_meditation       = distrib(gen);
    m_packet[m_pos ++] = m_meditation;
    generatePacketlen();
}

/**
 * @brief Generator::generateChecksum
 */
void Generator::generateChecksum() {
    m_checksum = 0;
    for(uint16_t i = 4; i < PACKETLEN && i < m_pos; i++) {
        m_checksum += m_packet[i];
    }

    m_checksum         = ~m_checksum & 0xFF;
    m_packet[m_pos ++] =  m_checksum;
}

/**
 * @brief Generator::generateAsicEeg
 */
void Generator::generateAsicEeg() {
    if(m_enable) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 0xFFFFFF);
        m_packet[m_pos ++] = 0x83;
        m_packet[m_pos ++] = 0x18;
        for(uint16_t i = 0; i < EEGPOINTS; i++) {
            m_eeg.at(i)        = distrib(gen);
            m_packet[m_pos ++] =  m_eeg.at(i) & 0xFF;
            m_packet[m_pos ++] = (m_eeg.at(i) >> 8)  & 0xFF;
            m_packet[m_pos ++] = (m_eeg.at(i) >> 16) & 0xFF;
        }
        generatePacketlen();
    }
}


/**
 * @brief Generator::setEnable
 * @param enable
 */
void Generator::startReceive() {
    m_enable = true;
}

/**
 * @brief Generator::stopReceive
 */
void Generator::stopReceive() {
    m_enable = false;
}
