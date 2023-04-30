#include "TGParser.h"
#include "../Models/EEGNormDataModel.h"

/* Decoder states (Packet decoding) */
#define PARSER_STATE_NULL           0x00  /* NULL state */
#define PARSER_STATE_SYNC           0x01  /* Waiting for SYNC byte */
#define PARSER_STATE_SYNC_CHECK     0x02  /* Waiting for second SYNC byte */
#define PARSER_STATE_PAYLOAD_LENGTH 0x03  /* Waiting for payload[] length */
#define PARSER_STATE_PAYLOAD        0x04  /* Waiting for next payload[] byte */
#define PARSER_STATE_CHKSUM         0x05  /* Waiting for chksum byte */

/* Decoder states (2-byte raw decoding) */
#define PARSER_STATE_WAIT_HIGH      0x06  /* Waiting for high byte */
#define PARSER_STATE_WAIT_LOW       0x07  /* High r'cvd.  Expecting low part */

/* Other constants */
#define PARSER_SYNC_BYTE            0xAA  /* Syncronization byte */
#define PARSER_EXCODE_BYTE          0x55  /* EXtended CODE level byte */


TGParser::TGParser(uint8_t parserType, QObject *parent) :
    QObject(parent)
  , type(parserType)
  , m_parser(new PacketParser)
  , m_eegDataModel(new EEGNormDataModel(this))
  , m_sessionId(-1)
{
    reset(parserType);
    qRegisterMetaType<::uint8_t>("uint8_t");
    qRegisterMetaType<::uint32_t>("uint32_t");
    qRegisterMetaType<::uint32_t *>("uint32_t*");
    qRegisterMetaType<double *>("double *");
    qRegisterMetaType<EEGData *>("EEGData *");
    qRegisterMetaType<EEGData>("EEGData");

    eegResetMinMax();
}

TGParser::~TGParser() {

}


void TGParser::reset(uint8_t parserType) {
    /* Initialize the parser's state based on the parser type */
    switch( parserType ) {
        case( PARSER_TYPE_PACKETS ):
            state = PARSER_STATE_SYNC;
            break;
        case( PARSER_TYPE_2BYTERAW ):
            state = PARSER_STATE_WAIT_HIGH;
            break;
        default:
            break;
    }

    /* Save parser type */
    type = parserType;
    lastByte = 0;
    payloadLength = 0;
    payloadBytesReceived = 0;
    memset(payload, 0, 0xFF);
    payloadSum = 0;
    chksum = 0;
    eegPoorSignal = 200;
    eegResetMinMax();
}

int TGParser::parseNext(uint8_t byte) {
    // m_parser->next(byte);

    int returnValue = 0;

    /* Pick handling according to current state... */
    switch( state ) {

        /* Waiting for SyncByte */
        case( PARSER_STATE_SYNC ):
            if( byte == PARSER_SYNC_BYTE ) {
                state = PARSER_STATE_SYNC_CHECK;
            }
            break;

        /* Waiting for second SyncByte */
        case( PARSER_STATE_SYNC_CHECK ):
            if( byte == PARSER_SYNC_BYTE ) {
                state = PARSER_STATE_PAYLOAD_LENGTH;

            } else {
                state = PARSER_STATE_SYNC;
            }
            break;

        /* Waiting for Data[] length */
        case( PARSER_STATE_PAYLOAD_LENGTH ):
            payloadLength = byte;
            if( payloadLength > 170 ) {
                state = PARSER_STATE_SYNC;
                returnValue = -3;
            } else if( payloadLength == 170 ) {
                returnValue = -4;
            } else {
                payloadBytesReceived = 0;
                payloadSum = 0;
                state = PARSER_STATE_PAYLOAD;
            }
            break;

        /* Waiting for Payload[] bytes */
        case( PARSER_STATE_PAYLOAD ):
            payload[payloadBytesReceived++] = byte;
            payloadSum = (uint8_t)(payloadSum + byte);
            if( payloadBytesReceived >= payloadLength ) {
                state = PARSER_STATE_CHKSUM;
            }
            break;

        /* Waiting for CKSUM byte */
        case( PARSER_STATE_CHKSUM ):
            chksum = byte;
            state = PARSER_STATE_SYNC;
            if( chksum != ((~payloadSum)&0xFF) ) {
                // qDebug("Checksum: 0x%02X 0x%02X", chksum, (~payloadSum)&0xFF);
                returnValue = -2;
            } else {
                returnValue = 1;
                packetPayload();
            }
            break;

        /* Waiting for high byte of 2-byte raw value */
        case( PARSER_STATE_WAIT_HIGH ):

            /* Check if current byte is a high byte */
            if( (byte & 0xC0) == 0x80 ) {
                /* High byte recognized, will be saved as lastByte */
                state = PARSER_STATE_WAIT_LOW;
            }
            break;

        /* Waiting for low byte of 2-byte raw value */
        case( PARSER_STATE_WAIT_LOW ):

            /* Check if current byte is a valid low byte */
            if( (byte & 0xC0) == 0x40 ) {

                /* Stuff the high and low part of the raw value into an array */
                payload[0] = lastByte;
                payload[1] = byte;

                /* Notify the handler function of received raw value */
                handleDataValue( 0, PARSER_CODE_RAW_SIGNAL, 2, payload);
                returnValue = 1;
            }

            /* Return to start state waiting for high */
            state = PARSER_STATE_WAIT_HIGH;

            break;

        /* unrecognized state */
        default:
            state = PARSER_STATE_SYNC;
            returnValue = -5;
            break;
    }

    /* Save current byte */
    lastByte = byte;

    return( returnValue );
}


/**
 * Parses each row of data from the @c packet's Data[] block,
 * updating the fields of @c data as appropriate.
 */
int TGParser::packetPayload() {
    uint8_t i = 0;
    uint8_t extendedCodeLevel = 0;
    uint8_t code = 0;
    uint8_t numBytes = 0;

    /* Parse all bytes from the payload[] */
    while( i < payloadLength ) {

        /* Parse possible EXtended CODE bytes */
        while( payload[i] == PARSER_EXCODE_BYTE ) {
            extendedCodeLevel++;
            i++;
        }

        /* Parse CODE */
        code = payload[i++];

        /* Parse value length */
        if( code >= 0x80 ) numBytes = payload[i++];
        else               numBytes = 1;

        /* Call the callback function to handle the DataRow value */
        handleDataValue( extendedCodeLevel, code, numBytes, payload+i );
        i = (uint8_t)(i + numBytes);
    }

    return( 0 );
}

/**
 * @brief TGParser::toShort
 * @param value
 * @return
 */
short TGParser::toShort(const uint8_t *value, uint8_t len) {
    if(len == 2) {
        return value[1] << 8 | (value[0] & 0xFF);
    }
    return -1;
}

/**
 * @brief TGParser::toUShort
 * @param value
 * @return
 */
unsigned short TGParser::toUShort(const uint8_t *value, uint8_t len) {
    if(len == 2) {
        return value[1] << 8 | (value[0] & 0xFF);
    }
    return -1;
}

/**
 * @brief TGParser::toFloat
 * @param value
 * @param len
 * @return
 */
float TGParser::toFloat(const uint8_t *value, uint8_t len) {
    if(len == 4) {
        return (value[3] << 24) | (value[3] << 16) | (value[2] << 8) | (value[3] & 0xFF)  ;
    }

    return -1.0;
}

/**
 * @brief TGParser::toUint32
 * @param value
 * @param len
 * @return
 */
uint32_t TGParser::toUint32(const uint8_t *value, uint8_t len) {
    if(len == 3) {
        return (value[2] << 16) | (value[1] << 8) | (value[0] & 0xFF);
    }

    return -1;
}

/**
 * @brief TGParser::eegResetMinMax
 */
void TGParser::eegResetMinMax() {
    for(uint8_t i = 0; i < EEG_SIZE; i++) {
        m_min[i] = 0xFFFFFFFF;
        m_max[i] = 0;
    }
    eegGlobalMax = 0;
}

/**
 * @brief TGParser::readEeg
 * @param value
 * @param eeg
 */
void TGParser::readEegPower(const uint8_t *value, uint8_t len) {
    if(len == 0x20) {
        ::uint32_t  eeg[EEG_SIZE];
        for (uint8_t i = 0; i < EEG_SIZE; i ++) {
            eeg[i] = toUint32(&value[i*3], 3);
        }

        EEGData data (eeg, m_min, m_max);
        emit setEegPower(data);
    }
}

/**
 *
 * @param data
 */
void TGParser::calcMinMax(::uint32_t data[]) {
    for (::uint8_t i = 0; i < EEG_SIZE; i++) {
        m_min[i] = data[i] < m_min[i] ? data[i] : m_min[i];
        m_max[i] = data[i] > m_max[i] ? data[i] : m_max[i];
    }
}


/**
 * @brief TGParser::getGlobalMax
 * @param globalMax
 */
void TGParser::getGlobalMax(uint32_t &globalMax) {
   globalMax = eegGlobalMax;
}

/**
 * @brief TGParser::readAsicEegPower
 * @param value
 * @param len
 */
void TGParser::readAsicEegPower(const uint8_t *value, uint8_t len) {
    if(len == 24) {
        ::uint32_t eeg[EEG_SIZE];
        for(uint8_t i = 0; i < 8; i++) {
            eeg[i] = (value[i * 3 + 2] << 16) | (value[i * 3 + 1] << 8) | (value[i * 3] & 0xFF);
        }
        calcMinMax(eeg);
        EEGData eegData(eeg, m_min, m_max);
        m_eegDataModel->addData(eegData, m_sessionId);
        if ( eegData.isDoubleDataValid()) {
            emit setEegAsicPower(eegData);
        }
    }
}

/**
 * @brief TGParser::handleDataValue
 * @param extendedCodeLevel
 * @param code
 * @param numBytes
 * @param value
 * @param customData
 * @return
 */
void TGParser::handleDataValue(uint8_t extendedCodeLevel,
                    uint8_t code,
                    uint8_t numBytes,
                    const uint8_t *value) {
    // qDebug("extended code level %d, code 0x%02X, num bytes %d", extendedCodeLevel, code, numBytes);
    if (extendedCodeLevel == 0){
        switch (code) {
            // Power
            case 0x01: {
                   emit setPower(value[0] & 0xFF);
                } break;
            // Poor Signal
            case 0x02: {
                    eegPoorSignal = (value[0] & 0xFF);
                    qDebug("EEG Poor Signal 0x%02X (%d)", eegPoorSignal, eegPoorSignal);
                    emit setPoorSignal(eegPoorSignal);
                } break;
            // Attention
            case 0x04: {

                    emit setAttention(value[0] & 0xFF);
                    // qDebug() << "Attention" << (value[0] & 0xFF);
                } break;
            // Meditation
            case 0x05: {
                    emit setMeditation(value[0] & 0xFF);
                    // qDebug() << "Attention" << (value[0] & 0xFF);
                } break;
            case 0x16:
                 emit setBlinkStrength(value[0] & 0xFF);
                 break;
            case 0xd4: {
                    // tr->sendMessage("Standby... autoconnecting");
                    QByteArray bytes;
                        bytes.append(0xC2);
                        emit setWriteBytes(bytes);
                    } break;
            case( 0xd0 ):
                        emit setReady();
                    break;
            case( 0xd1 ): {
                        emit setError(0xd1);
                    } break;
            case 0x80:
                emit setRaw(toShort(value, numBytes));
                // qDebug() << QByteArray((char*) value, numBytes).toHex();
                break;
            case 0x82:

                break;
            case 0x81:
                readEegPower(value, numBytes);
                qDebug() << "EEG Power";
            break;
            case 0x83:
                readAsicEegPower(value, numBytes);
                qDebug() << "Asic EEG Power";
            break;
            // Other [CODE]s
            case 0x86: {
                emit setPrInterval(toShort(value, numBytes));
            } break;
            default: {
                emit setUnknownPacket(code, value, numBytes);
            } break;
        }
    }
}
