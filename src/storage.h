#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <inttypes.h>
#include <RTClib.h>

#include "logger.h"
#include "measures.h"

#define HEADER_START 0x00
#define HEADER_END (sizeof(Storage::Header) * 12)
#define MEASURES_START HEADER_END
#define MEASURES_END EEPROM.end() - 1
// TODO: replace with real month number from RTC
#define RESET_TIME false
#define TEMP_MEASURES_NUMBER 6

class Storage {
public:
    void addMeasures(Measures const &measures);

    Measures getLastMeasures();

    Measures *getTempMeasures();

    static Storage &getInstance();

    Storage(Storage const &) = delete;

    void operator=(Storage const &) = delete;

private:
    Storage();

    struct CompactMeasures {
        uint8_t ppm;
        uint8_t temp;
        uint8_t hum;
        uint8_t press;
    };

    struct Header {
        uint32_t crc;
        uint8_t lastIndex;
        uint16_t hourNumber;
        uint8_t rest;          // TODO: store something useful
        uint32_t headerCrc;
    };

    Logger logger;
    Header header;
    Measures tempMeasures[TEMP_MEASURES_NUMBER];
    uint8_t tempMeasuresNumber = 0; // temp means temporary, but NOT temperature
    RTC_DS3231 rtc;

    uint8_t putCompactMeasures(CompactMeasures &);

    CompactMeasures getCompactMeasures(uint16_t);

    CompactMeasures measuresToCompactMeasures(Measures const &);

    uint32_t getCRC(uint16_t, uint16_t);

    uint32_t getCRC();

    uint32_t getCRC(Header &);

    Measures compactMeasuresToMeasures(CompactMeasures &);

    Header getLastHeader();

    uint16_t getCurrentHourNumber();

    uint8_t getCurrentHeaderIndex();

    void clear();

    Header computeHeader(uint8_t);

    void saveHeader();

    uint16_t getCurrentHeaderPtr();

    bool checkHeader(Header &);

    uint32_t crcTick(uint32_t, uint8_t);
};

//static StorageClass Storage;