
#include <RTClib.cpp>

#include "storage.h"

Storage::Storage() : logger(new Logger(F("Storage"))), header(this->getLastHeader()) {
    if (!this->rtc.begin()) {
        this->logger->error(F("Could not set up an RTC module"));
        while (1);  // TODO: display error message on screen
    }

    this->rtc.begin();

    if (RESET_TIME || this->rtc.lostPower()) {
        RTC_DS3231::adjust(DateTime(F(__DATE__), F(__TIME__)));
        this->logger->info("Time was adjusted:", F(__DATE__), F(__TIME__));
    }

    if (!this->checkHeader(header)) {
        this->logger->error(F("Header corrupted"));
        this->clear();
        this->header = this->computeHeader(0);
        this->saveHeader();
    }

    if (this->header.crc != this->getCRC()) {
        this->logger->error(F("EEPROM data corrupted. header.crc:"), this->header.crc, F("; actual crc:"), this->getCRC());
        this->clear();
        this->header = this->computeHeader(0);
        this->saveHeader();
    } else {
        this->logger->info(F("EEPROM data is ok! CRC:"), this->header.crc);
    }
}

Storage &Storage::getInstance() {
    static Storage instance;
    return instance;
}

void Storage::addMeasures(Measures const &measures) {
    Wire.begin();
    uint8_t currentMinute = RTC_DS3231::now().minute();
//    if(currentMinute % (60 / TEMP_MEASURES_NUMBER) != 0 || currentMinute == lastMinuteNumber) {
    if(currentMinute == lastMinuteNumber) {
        this->logger->info(F("Skip measures. Last minute:"), this->lastMinuteNumber);
        return;
    }

    if (this->getCurrentHourNumber() > this->header.hourNumber) {
        this->logger->info(F("Current hour is over. Put values to EEPROM"));

        uint16_t ppm = 0;   // up to 13 measures!!!
        float temp = 0.0f;     // up to ~ inf measures
        uint16_t humidity = 0;  // up to 655 measures
        uint16_t pressHg = 0;   // up to ~82 measures

        for (uint8_t i = 0; i < this->tempMeasuresNumber; ++i) {
            ppm += this->tempMeasures[i].ppm;
            temp += this->tempMeasures[i].temp;
            humidity += this->tempMeasures[i].humidity;
            pressHg += this->tempMeasures[i].pressHg;
        }

        CompactMeasures cm = this->measuresToCompactMeasures(Measures(
                ppm / this->tempMeasuresNumber,
                temp / float (this->tempMeasuresNumber),
                humidity / this->tempMeasuresNumber,
                pressHg / this->tempMeasuresNumber
        ));

        this->tempMeasuresNumber = 0;

        this->putCompactMeasures(cm);
    }

    this->lastMinuteNumber = currentMinute;

    for(uint8_t i = 0; i < this->tempMeasuresNumber - 1; ++i) {
        this->tempMeasures[i] = this->tempMeasures[i + 1];
    }

    this->tempMeasuresNumber = this->tempMeasuresNumber + 1 > TEMP_MEASURES_NUMBER ? TEMP_MEASURES_NUMBER : this->tempMeasuresNumber + 1;
    this->tempMeasures[this->tempMeasuresNumber - 1] = measures;

    for(uint8_t i = 0; i < 6; i++) {
        this->logger->info(this->tempMeasures[i]);
    }
}

Storage::CompactMeasures Storage::measuresToCompactMeasures(Measures const &measures) {
    return {
            uint8_t((measures.ppm - 410) / 18),
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
    uint16_t hourNumber = 0;
    for (uint8_t i = 0; i < 12; ++i, headerPtr += sizeof(Header)) {
        hourNumber = EEPROM.read(uint16_t(headerPtr + offsetof(struct Header, hourNumber)));
        if (hourNumber > header.hourNumber) {
            EEPROM.get(headerPtr, header);
        } else if (hourNumber < header.hourNumber){
            break;
        }
    }

    return header;
}

Measures Storage::compactMeasuresToMeasures(Storage::CompactMeasures &measures) {
    return Measures(
            uint16_t(measures.ppm * 18 + 410),
            float(measures.temp) / 255 * 30 + 10,
            measures.hum,
            uint16_t(measures.press + 700)
    );
}


Measures Storage::getLastMeasures() {
    auto compactMeasures = this->getCompactMeasures(this->tempMeasuresNumber);
    return this->compactMeasuresToMeasures(compactMeasures);
}


Measures* Storage::getTempMeasures() {
    return this->tempMeasures;
}

uint32_t Storage::crcTick(uint32_t crc, uint8_t byte) {
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

uint32_t Storage::getCRC() {
    return this->getCRC(MEASURES_START, MEASURES_END);
}

uint32_t Storage::getCRC(uint16_t start, uint16_t end) {
    uint32_t crc = ~0LU;

    for (uint16_t index = start; index <= end; ++index) {
        if(EEPROM[index] != 0) {
            this->logger->info(F("Not null EEPROM data:"), EEPROM[index]);
        }
        crc = this->crcTick(crc, EEPROM[index]);
    }
    this->logger->info(F("Crc computed:"), start, end, crc);

    return crc;
}

uint32_t Storage::getCRC(Storage::Header &header) {
    uint32_t crc = ~0LU;

    uint8_t *headerBytes = reinterpret_cast<uint8_t *>(&header);
    for (auto i = 0; i < offsetof(struct Header, headerCrc); ++i) {
        crc = this->crcTick(crc, headerBytes[i]);
    }
    this->logger->info(F("header crc computed:"), crc);

    return crc;
}


void Storage::clear() {
    for (auto i = EEPROM.begin(); i < EEPROM.end(); ++i) {
        EEPROM.update(i, 0);
    }
    this->logger->info(F("EEPROM was cleared"));
}

uint8_t Storage::putCompactMeasures(CompactMeasures &measures) {
    uint16_t ptr = MEASURES_START + this->header.lastIndex * sizeof(measures);
    uint8_t lastIndex = this->header.lastIndex;

    if (ptr + sizeof(measures) >= MEASURES_END) {
        ptr = MEASURES_START;
        lastIndex = 0;
    } else {
        ++lastIndex;
    }

    EEPROM.put(ptr, measures);

    this->header = this->computeHeader(lastIndex);
    this->saveHeader();

    this->logger->info(F("Measures were written to EEPROM. LastIndex:"), lastIndex);

    return lastIndex; // TODO: return something more useful
}

Storage::CompactMeasures Storage::getCompactMeasures(uint16_t index) {
    uint16_t measurePtr = MEASURES_START + index * sizeof(CompactMeasures);
    CompactMeasures cm{};
    EEPROM.get(measurePtr, cm);
    return cm;
}

uint16_t Storage::getCurrentHourNumber() {
    this->logger->info(F("Compute current hour number"));
    const auto shift = DateTime(2019, 12, 1);
    this->logger->info(F("2020 shift:"), shift.secondstime());
    Wire.begin();
    DateTime now = RTC_DS3231::now();
    this->logger->info(F("Current shift:"), now.secondstime());
    return (now.secondstime() - shift.secondstime()) / 3600;
}

uint8_t Storage::getCurrentHeaderIndex() {
    Wire.begin();
    return RTC_DS3231::now().month() - 1;
}


Storage::Header Storage::computeHeader(uint8_t lastIndex) {
    Header header{};
    header.crc = this->getCRC();
    header.hourNumber = this->getCurrentHourNumber();
    header.lastIndex = lastIndex;
    header.rest = 0U;
    header.headerCrc = this->getCRC(header);
    return header;
}

uint16_t Storage::getCurrentHeaderPtr() {
    return HEADER_START + (this->getCurrentHeaderIndex() * sizeof(Header));
}

void Storage::saveHeader() {
    this->logger->info(F("Save header at index"), this->getCurrentHeaderIndex());
    EEPROM.put(this->getCurrentHeaderPtr(), this->header);

    Header header = this->getLastHeader();
    if(this->header.crc != header.crc || this->header.hourNumber != header.hourNumber || this->header.headerCrc != header.headerCrc) {
        this->logger->error(F("Header save error"));
    }
//    this->logger->info(F(""))
}

bool Storage::checkHeader(Storage::Header &header) {
    this->logger->info(F("Header. lastIndex:"), this->header.lastIndex, F("crc:"), this->header.crc);
    uint32_t headerCrc = this->getCRC(header);
    return header.headerCrc == headerCrc;
}

