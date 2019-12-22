#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <inttypes.h>

#include "measures.h"
#include "logger.h"

#define HEADER_START 0x00
#define HEADER_END (sizeof(StorageClass::Header) * 12)
#define MEASURES_START HEADER_END
#define MEASURES_END EEPROM.end()
// TODO: replace with real month number from RTC
#define MONTH_NUM 0
// TODO: define header rotation size (and replace '12' with it)

class StorageClass {
public:
    StorageClass();
    void addMeasures(Measures const &measures);
    Measures getLastMeasures();

    StorageClass(StorageClass const&) = delete;
    void operator=(StorageClass const&)  = delete;
private:

    struct CompactMeasures {
        uint8_t ppm;
        uint8_t temp;
        uint8_t hum;
        uint8_t press;
    };

    struct Header {
        uint32_t crc;
        uint8_t lastIndex;
        uint8_t monthNumber;
        uint16_t rest;          // TODO: store something useful
        uint32_t headerCrc;
    };

    Logger logger;
    Header header;
    Measures lastHourStorage[6];
    uint8_t lastMeasuresIndex = 0;

    uint8_t putCompactMeasures(CompactMeasures&);
    CompactMeasures getCompactMeasures(uint16_t);
    CompactMeasures measuresToCompactMeasures(Measures const &);
    uint32_t getCRC(uint16_t, uint16_t);
    uint32_t getCRC();
    uint32_t getCRC(Header&);
    Measures compactMeasuresToMeasures(CompactMeasures&);
    Header getLastHeader();
    uint8_t getCurrentMonthNumber();
    void clear();
    Header computeHeader(uint8_t);
    void saveHeader();
    uint16_t getCurrentHeaderPtr();
    bool checkHeader(Header&);
    uint32_t crcTick(uint32_t, uint8_t);
};

static StorageClass Storage;