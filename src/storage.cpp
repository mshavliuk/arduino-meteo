
#include "storage.h"

StorageClass::StorageClass() : logger(F("Storage")), header(this->getLastHeader()) {
    if (!this->checkHeader(header)) {
        this->logger.error(F("Header corrupted"));
        this->clear();
        this->header = this->computeHeader(0);
        this->saveHeader();
    }

    if (this->header.crc != this->getCRC()) {
        this->logger.error(F("EEPROM data corrupted"));
        this->clear();
        this->header = this->computeHeader(0);
        this->saveHeader();
    } else {
        this->logger.info(F("EEPROM data is ok! CRC:"), this->header.crc);
    }
}

void StorageClass::addMeasures(Measures const &measures) {
    // put into RAM
    this->lastHourStorage[this->lastMeasuresIndex] = measures;
    CompactMeasures cm = this->measuresToCompactMeasures(measures);
    this->putCompactMeasures(cm);
    this->logger.info(F("Added measures"), measures);

    // if we have 6 measures (for 1 hour) put the average measures into EEPROM
}

StorageClass::CompactMeasures StorageClass::measuresToCompactMeasures(Measures const &measures) {
    return {
            uint8_t((measures.ppm - 410) / 18),
            uint8_t((measures.temp - 10) / 30 * 255),
            measures.humidity,
            uint8_t(measures.pressHg - 700),
    };
}

StorageClass::Header StorageClass::getLastHeader() {
    uint16_t headerPtr = HEADER_START;
    Header header{};
    EEPROM.get(headerPtr, header);
    headerPtr += sizeof(Header);
    uint8_t monthNumber;
    for (uint8_t i = 1; i < 12; ++i, headerPtr += sizeof(Header)) {
        monthNumber = EEPROM.read(uint16_t(headerPtr + offsetof(struct Header, monthNumber)));
        if (monthNumber > header.monthNumber) {
            EEPROM.get(headerPtr, header);
        } else {
            break;
        }
    }

    return header;
}

Measures StorageClass::compactMeasuresToMeasures(StorageClass::CompactMeasures &measures) {
    return Measures(
            uint16_t(measures.ppm * 18 + 410),
            float(measures.temp) / 255 * 30 + 10,
            measures.hum,
            uint16_t(measures.press + 700)
    );
}


Measures StorageClass::getLastMeasures() {
    auto compactMeasures = this->getCompactMeasures(this->lastMeasuresIndex);
    return this->compactMeasuresToMeasures(compactMeasures);
}

uint32_t StorageClass::crcTick(uint32_t crc, uint8_t byte) {
    const unsigned long crc_table[16] = {
            0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
            0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
            0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
            0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
    };


    crc = crc_table[(crc ^ byte) & 0x0fU] ^ (crc >> 4U);
    crc = crc_table[(crc ^ (byte >> 4U)) & 0x0fU] ^ (crc >> 4U);
    return ~crc;
}

uint32_t StorageClass::getCRC() {
    return this->getCRC(MEASURES_START, EEPROM.length());
}

uint32_t StorageClass::getCRC(uint16_t start, uint16_t end) {
    uint32_t crc = ~0LU;

    for (uint16_t index = start; index < end; ++index) {
        crc = this->crcTick(crc, EEPROM[index]);
    }
    return crc;
}

uint32_t StorageClass::getCRC(StorageClass::Header &header) {
    uint32_t crc = ~0LU;

    uint8_t *headerBytes = reinterpret_cast<uint8_t *>(&header);
    for (auto i = 0; i < offsetof(struct Header, headerCrc); ++i) {
        crc = this->crcTick(crc, headerBytes[i]);
    }

    return crc;
}


void StorageClass::clear() {
    for (auto i = EEPROM.begin(); i < EEPROM.end(); ++i) {
        EEPROM.update(i, 0);
    }
    this->logger.info(F("EEPROM was cleared"));
}

uint8_t StorageClass::putCompactMeasures(CompactMeasures &measures) {
    uint16_t ptr = MEASURES_START + this->header.lastIndex * sizeof(measures);
    uint8_t lastIndex = this->header.lastIndex;

    if (ptr + sizeof(measures) >= MEASURES_END) {
        ptr = MEASURES_START;
        lastIndex = 0;
    } else {
        ++lastIndex;
    }

    this->header = this->computeHeader(lastIndex);
    this->saveHeader();

    this->logger.info(F("Measures were written to EEPROM. LastIndex:"), lastIndex);

    return lastIndex; // TODO: return something more useful
}

StorageClass::CompactMeasures StorageClass::getCompactMeasures(uint16_t index) {
    uint16_t measurePtr = MEASURES_START + index * sizeof(CompactMeasures);
    CompactMeasures cm{};
    EEPROM.get(measurePtr, cm);
    return cm;
}

uint8_t StorageClass::getCurrentMonthNumber() {
    return MONTH_NUM;
}

StorageClass::Header StorageClass::computeHeader(uint8_t lastIndex) {
    Header header{};
    header.crc = this->getCRC();
    header.monthNumber = this->getCurrentMonthNumber();
    header.lastIndex = lastIndex;
    header.headerCrc = this->getCRC(header);
    header.rest = 0U;
    return header;
}

uint16_t StorageClass::getCurrentHeaderPtr() {
    return HEADER_START + (this->getCurrentMonthNumber() * sizeof(Header));
}

void StorageClass::saveHeader() {
    EEPROM.put(this->getCurrentHeaderPtr(), this->header);
}

bool StorageClass::checkHeader(StorageClass::Header &header) {
    uint32_t headerCrc = this->getCRC(header);
    return header.headerCrc == headerCrc;
}

