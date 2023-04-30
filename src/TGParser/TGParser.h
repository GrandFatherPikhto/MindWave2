#pragma once

#include <QObject>
#include <QDataStream>
#include <QDebug>

#include <vector>
#include <queue>
#include <deque>

#include "PacketParser.h"
#include "EEGData.h"

using namespace std;

QT_FORWARD_DECLARE_CLASS(EEGNormDataModel);

/* PacketParser types */
#define PARSER_TYPE_NULL       0x00
#define PARSER_TYPE_PACKETS    0x01    /* Stream bytes as ThinkGear Packets */
#define PARSER_TYPE_2BYTERAW   0x02    /* Stream bytes as 2-byte raw data */

/* Data CODE definitions */
#define PARSER_CODE_BATTERY            0x01
#define PARSER_CODE_POOR_QUALITY       0x02
#define PARSER_CODE_ATTENTION          0x04
#define PARSER_CODE_MEDITATION         0x05
#define PARSER_CODE_8BITRAW_SIGNAL     0x06
#define PARSER_CODE_RAW_MARKER         0x07

#define PARSER_CODE_RAW_SIGNAL         0x80
#define PARSER_CODE_EEG_POWERS         0x81
#define PARSER_CODE_ASIC_EEG_POWER_INT 0x83

#define BAD_SIGNAL                     200

#define QUEUE_SIZE                     0x400

class TGParser : public QObject
{
    Q_OBJECT
public:
    explicit TGParser(uint8_t parserType = PARSER_TYPE_PACKETS, QObject *parent = nullptr);
    ~TGParser();
    void reset(uint8_t);
    short toShort(const uint8_t *, uint8_t);
    unsigned short toUShort(const uint8_t *, uint8_t);
    float toFloat(const uint8_t *, uint8_t);
    uint32_t toUint32(const uint8_t *, uint8_t);
    void readAsicEegPower(const uint8_t *, uint8_t);
    void readEegPower(const uint8_t *, uint8_t);
    int parseNext(uint8_t);
    int packetPayload();
    void handleDataValue(uint8_t extendedCodeLevel,
                        uint8_t code,
                        uint8_t numBytes,
                        const uint8_t *value);
    void calcMinMax(::uint32_t data[]);
    void getGlobalMax(uint32_t &);
    void eegResetMinMax();

    inline void setSessionId(int id) {
        m_sessionId = id;
    }

signals:
    void setPower(uint8_t);
    void setEegPower(uint8_t);
    void setPoorSignal(uint8_t);
    void setAttention(uint8_t);
    void setMeditation(uint8_t);
    void setBlinkStrength(uint8_t);
    void setWriteBytes(const QByteArray &);
    void sendMessage(const QString &);
    void setRaw(short);
    void setPrInterval(short);
    void setEegPower(EEGData & eegData);
    void setEegAsicPower(EEGData & eegAsicData);
    void setUnknownPacket(uint8_t, const uint8_t *, uint8_t);
    void setError(uint8_t);
    void setReady();


private:
    void calcEeg(const ::uint32_t eeg[]);
    void writeEEGData(const ::uint32_t eeg[]);

    uint8_t   type;
    uint8_t   state;

    uint8_t   lastByte;

    uint8_t   payloadLength;
    uint8_t   payloadBytesReceived;
    uint8_t   payload[256];
    uint8_t   payloadSum;
    uint8_t   chksum;

    ::uint32_t m_min[EEG_SIZE];
    ::uint32_t m_max[EEG_SIZE];

    uint32_t eegGlobalMax;
    uint8_t  eegPoorSignal;

    PacketParser *m_parser;

    EEGNormDataModel *m_eegDataModel;

    int m_sessionId;
};

