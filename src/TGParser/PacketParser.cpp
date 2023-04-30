#include "PacketParser.h"

PacketParser::PacketParser(QObject *parent) :
    QObject(parent)
{

}

PacketParser::~PacketParser() {

}

void PacketParser::reset() {
    m_vlen = 0;
    m_pos  = 0;
    m_plen = 0;
    m_code = 0;
    memset(m_packet, 0, PACKET_LEN);
}

/** @brief PacketParser::next
 * @param bt
 */
void PacketParser::next(uint8_t bt) {
    // qDebug() << bt;
    if(m_pos == 0) {
        if(bt == 0xAA) {
            m_packet[m_pos++] = (char)bt;
        } else {
            reset();
        }
    } else if(m_pos == 1) {
        if(bt == 0xAA) {
            m_packet[m_pos++] = bt;
        }
    } else if (m_pos == 2) {
        m_packet[m_pos] = bt;
        m_plen = bt;
        m_pos ++;
    } else if(m_pos >= 3 && m_pos < m_plen + 5) {
        m_packet[m_pos++] = bt;
    } else if(m_pos >= m_plen + 5) {
        m_packet[m_pos++] = bt;
        QByteArray packet((char *)m_packet, m_plen + 5);
        qDebug() << packet.toHex();
        qDebug("code 0x%02X (%d), plen 0x%02X (%d)", m_code, m_code, m_plen, m_plen);
        parsePacket();
        reset();
    }
}


/**
 * @brief PacketParser::parsePacket
 */
void PacketParser::parsePacket() {
    for(uint8_t i = 0; i < m_plen; i++) {

    }
    qDebug("0x%02X", m_packet[m_plen + 1]);
}

