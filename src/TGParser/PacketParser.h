#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QDebug>

#define PACKET_LEN 0x100

class PacketParser : public QObject
{
    Q_OBJECT
public:
    explicit PacketParser(QObject *parent = nullptr);
    ~PacketParser();
    void next(uint8_t);
    void reset();
    void parsePacket();

signals:

private:
    uint8_t m_packet[PACKET_LEN];
    uint16_t m_pos;
    uint16_t m_plen;
    uint16_t m_vlen;
    uint8_t m_code;
    uint8_t m_checksum;
};

#endif // PARSER_H
