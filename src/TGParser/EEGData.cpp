//
// Created by denis on 24.04.2023.
//
#include <QDebug>
#include <cstring>
#include "EEGData.h"

EEGData::EEGData(const uint32_t eeg[], const ::uint32_t min[], const ::uint32_t max[]) {
    ::memcpy(m_eeg, eeg, EEG_SIZE * sizeof(::uint32_t));
    ::memcpy(m_min, min, EEG_SIZE * sizeof(::uint32_t));
    ::memcpy(m_max, max, EEG_SIZE * sizeof(::uint32_t));
    // calcData();
    calcWithoutMinData ();
}

EEGData::EEGData(const EEGData &data) {
    ::memcpy(m_eeg,  data.m_eeg,  EEG_SIZE * sizeof(::uint32_t));
    ::memcpy(m_min,  data.m_min,  EEG_SIZE * sizeof(::uint32_t));
    ::memcpy(m_max,  data.m_max,  EEG_SIZE * sizeof(::uint32_t));
    ::memcpy(m_data, data.m_data, EEG_SIZE * sizeof(double));
}

EEGData::~EEGData() {

}

const EEGData &EEGData::operator=(const EEGData &data) {
    ::memcpy(m_eeg,  data.m_eeg,  EEG_SIZE * sizeof(::uint32_t));
    ::memcpy(m_min,  data.m_min,  EEG_SIZE * sizeof(::uint32_t));
    ::memcpy(m_max,  data.m_max,  EEG_SIZE * sizeof(::uint32_t));
    ::memcpy(m_data, data.m_data, EEG_SIZE * sizeof(double));
    return *this;
}


