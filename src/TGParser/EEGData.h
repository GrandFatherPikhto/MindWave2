//
// Created by denis on 24.04.2023.
//
#pragma once

#include <cstdint>

#define EEG_SIZE 0x08

class EEGData {
public:

    typedef struct {
        double delta;
        double theta;
        double loAlpha;
        double hiAlpha;
        double loBeta;
        double hiBeta;
        double loGamma;
        double midGamma;
    } DOUBLE_EEG_DATA;

    typedef struct {
        uint32_t delta;
        uint32_t theta;
        uint32_t loAlpha;
        uint32_t hiAlpha;
        uint32_t loBeta;
        uint32_t hiBeta;
        uint32_t loGamma;
        uint32_t midGamma;
    } UINT32_EEG_DATA;


    typedef enum {
        Delta       = 0x0,
        Theta       = 0x01,
        LowAlpha    = 0x02,
        HighAlpha   = 0x03,
        LowBeta     = 0x04,
        HighBeta    = 0x05,
        LowGamma    = 0x06,
        MiddleGamma = 0x07
    } EEG_DATA;

    EEGData(const ::uint32_t eeg[], const ::uint32_t min[], const ::uint32_t max[]);
    EEGData(const EEGData &data);
    ~EEGData();

    const EEGData & operator = (const EEGData &data);

    inline void printDoubleData() {
        qDebug() << "Double Data: " << m_data[0] << m_data[1] << m_data[2] << m_data[3] << m_data[4] << m_data[5] << m_data[6] << m_data[7];
    }

    inline double delta     () { return m_data[Delta]; }
    inline double theta     () { return m_data[Theta]; }
    inline double lowAlpha  () { return m_data[LowAlpha]; }
    inline double highAlpha () { return m_data[HighAlpha]; }
    inline double lowBeta   () { return m_data[LowBeta]; }
    inline double highBeta  () { return m_data[HighBeta]; }
    inline double lowGamma  () { return m_data[LowGamma]; }
    inline double midGamma  () { return m_data[MiddleGamma]; }

    inline double eegDelta     () { return m_eeg[Delta]; }
    inline double eegTheta     () { return m_eeg[Theta]; }
    inline double eegLowAlpha  () { return m_eeg[LowAlpha]; }
    inline double eegHighAlpha () { return m_eeg[HighAlpha]; }
    inline double eegLowBeta   () { return m_eeg[LowBeta]; }
    inline double eegHighBeta  () { return m_eeg[HighBeta]; }
    inline double eegLowGamma  () { return m_eeg[LowGamma]; }
    inline double eegMidGamma  () { return m_eeg[MiddleGamma]; }

    inline double minDelta     () { return m_min[Delta]; }
    inline double minTheta     () { return m_min[Theta]; }
    inline double minLowAlpha  () { return m_min[LowAlpha]; }
    inline double minHighAlpha () { return m_min[HighAlpha]; }
    inline double minLowBeta   () { return m_min[LowBeta]; }
    inline double minHighBeta  () { return m_min[HighBeta]; }
    inline double minLowGamma  () { return m_min[LowGamma]; }
    inline double minMidGamma  () { return m_min[MiddleGamma]; }

    inline double maxDelta     () { return m_max[Delta]; }
    inline double maxTheta     () { return m_max[Theta]; }
    inline double maxLowAlpha  () { return m_max[LowAlpha]; }
    inline double maxHighAlpha () { return m_max[HighAlpha]; }
    inline double maxLowBeta   () { return m_max[LowBeta]; }
    inline double maxHighBeta  () { return m_max[HighBeta]; }
    inline double maxLowGamma  () { return m_max[LowGamma]; }
    inline double maxMidGamma  () { return m_max[MiddleGamma]; }

    inline ::uint32_t* getEeg() { return m_eeg; }
    inline ::uint32_t* getMin() { return m_min; }
    inline ::uint32_t* getMax() { return m_max; }


    inline void calcData () {
        for(uint8_t i = 0; i < EEG_SIZE; i++) {
            m_data[i] =  static_cast<double>(m_eeg[i] - m_min[i])
                        /static_cast<double>(m_max[i] - m_min[i]);
        }
    }

    inline void calcWithoutMinData () {
        for(uint8_t i = 0; i < EEG_SIZE; i++) {
            m_data[i] =  static_cast<double>(m_eeg[i])
                         /static_cast<double>(m_max[i]);
        }
    }

    inline const DOUBLE_EEG_DATA & getData() {
        m_doubleData.delta    = m_data[Delta];
        m_doubleData.theta    = m_data[Theta];
        m_doubleData.loAlpha  = m_data[LowAlpha];
        m_doubleData.hiAlpha  = m_data[HighAlpha];
        m_doubleData.loBeta   = m_data[LowBeta];
        m_doubleData.hiBeta   = m_data[HighBeta];
        m_doubleData.loGamma  = m_data[LowGamma];
        m_doubleData.midGamma = m_data[MiddleGamma];
        return m_doubleData;
    }

    inline bool isDoubleDataValid () {
        bool isValid = true;
        for (uint8_t i = 0; i < EEG_SIZE; i++) {
            isValid &= (m_data[i] >= 0 && m_data[i] <= 1.0);
        }
        return isValid;
    }

private:
    ::uint32_t m_eeg  [EEG_SIZE];
    ::uint32_t m_min  [EEG_SIZE];
    ::uint32_t m_max  [EEG_SIZE];
    double     m_data [EEG_SIZE];
    DOUBLE_EEG_DATA m_doubleData;
};


