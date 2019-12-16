#pragma once

#include <EEPROM.h>

#include "storage.h"
#include "sensors.cpp"

Storage::Storage() : logger(F("Storage")) {
//    EEPROM.read(reinterpret_cast<int>(this->ptr));

    this->header = this->getLastHeader();
//    uint32_t crc;
//    EEPROM.get(CRC_PTR, crc);
    if(this->header.crc != ~this->header.notCrc) {
        this->logger.error(F("CRC corrupted"));
        // TODO: try to restore by comparison with real crc
        this->clear();
        this->header = this->computeHeader(0);
    }

    if (this->header.crc != this->getCRC(MEASURES_START)) {
        this->logger.error(F("EEPROM data corrupted"));
        this->clear();
        this->header = this->computeHeader(0);
    } else {
        this->logger.info(F("EEPROM data is ok! CRC:"), this->header.crc);
    }

}

void Storage::addMeasures(Sensors::Measures measures) {
    // put into RAM
    this->lastHourStorage[this->lastHourMeasuresNumber] = measures;
    CompactMeasures cm = this->measuresToCompactMeasures(measures);
    Sensors::Measures m = this->compactMeasuresToMeasures(cm); // just to debug, TODO: remove
    this->putCompactMeasures(cm);

    // if we have 6 measures (for 1 hour) put the average measures into EEPROM
}

Storage::CompactMeasures Storage::measuresToCompactMeasures(Sensors::Measures measures) {
    return {
            uint8_t ((measures.ppm - 410) / 18),
            uint8_t((measures.temp - 10) / 30 * 255),
            measures.humidity,
            uint8_t(measures.pressHg - 700),
    };
}

Storage::Header Storage::getLastHeader() {
    uint16_t headerPtr = HEADER_START;
    Header header{};
    EEPROM.get(headerPtr, header);
    headerPtr += sizeof(Header);
    uint8_t monthNumber;
    for(uint8_t i = 1; i < 12; ++i, headerPtr += sizeof(Header)) {
        monthNumber = EEPROM.read(uint16_t (headerPtr + offsetof(struct Header, monthNumber)));
        if(monthNumber > header.monthNumber) {
            EEPROM.get(headerPtr, header);
        } else {
            break;
        }
    }

    return header;
}

Sensors::Measures Storage::compactMeasuresToMeasures(Storage::CompactMeasures measures) {
    return {
            uint16_t (measures.ppm * 18 + 410),
            float(measures.temp) / 255 * 30 + 10,
            measures.hum,
            uint16_t(measures.press + 700)
    };
}

uint32_t Storage::getCRC(uint16_t start = 0x00) {
    const unsigned long crc_table[16] = {
            0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
            0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
            0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
            0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
    };

    uint32_t crc = ~0LU;

    for (int index = start; index < EEPROM.length(); ++index) {
        crc = crc_table[(crc ^ EEPROM[index]) & 0x0fU] ^ (crc >> 4U);
        crc = crc_table[(crc ^ (EEPROM[index] >> 4U)) & 0x0fU] ^ (crc >> 4U);
        crc = ~crc;
    }
    return crc;
}

void Storage::clear() {
    for (auto i = EEPROM.begin(); i < EEPROM.end(); ++i) {
        EEPROM.update(i, 0);
    }
    this->logger.info(F("EEPROM was cleared"));
}

uint8_t Storage::putCompactMeasures(CompactMeasures measures) {
    uint16_t ptr = MEASURES_START + this->header.lastIndex * sizeof(measures);
    uint8_t lastIndex = this->header.lastIndex;

    if(ptr + sizeof(measures) >= MEASURES_END) {
        ptr = MEASURES_START;
        lastIndex = 0;
    } else {
        ++lastIndex;
    }

    this->header = this->computeHeader(lastIndex);
    EEPROM.put(this->getCurrentHeaderPtr(), this->header);

    this->logger.info(F("Measures were written to EEPROM. LastIndex:"), lastIndex);

    return lastIndex; // TODO: return something more useful
}

Sensors::Measures Storage::getLastMeasures() {
    uint16_t lastMeasurePtr = MEASURES_START + this->header.lastIndex * sizeof(CompactMeasures);

    return this->compactMeasuresToMeasures(this->getCompactMeasures(lastMeasurePtr));
}

Storage::CompactMeasures Storage::getCompactMeasures(uint16_t index) {
    CompactMeasures cm{};
    EEPROM.get(index, cm);
    return cm;
}

Storage &Storage::get() {
    static Storage instance;
    return instance;
}

uint8_t Storage::getCurrentMonthNumber() {
    return MONTH_NUM;
}

Storage::Header Storage::computeHeader(uint8_t lastIndex) {
    Header header{};
    header.crc = this->getCRC(MEASURES_START);
    header.notCrc = ~header.crc;
    header.monthNumber = this->getCurrentMonthNumber();
    header.lastIndex = lastIndex;
    return header;
}

uint16_t Storage::getCurrentHeaderPtr() {
    return HEADER_START + (this->getCurrentMonthNumber() * sizeof(Header));
}
