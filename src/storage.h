#pragma once

#include "logger.cpp"

#define HEADER_START 0x00
#define HEADER_END (sizeof(Storage::Header) * 12)
#define MEASURES_START HEADER_END
#define MEASURES_END EEPROM.end()
// TODO: replace with real month number from RTC
#define MONTH_NUM 0
// TODO: define header rotation size (and replace '12' with it)

class Storage {
public:
    static Storage& get();

    void addMeasures(Sensors::Measures measures);
    Sensors::Measures getLastMeasures();

    Storage(Storage const&) = delete;
    void operator=(Storage const&)  = delete;
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
        uint32_t notCrc;
        uint8_t lastIndex;
        uint8_t monthNumber;
        uint16_t rest;          // TODO: store something useful
    };

    // TODO: add header boundaries
    static const uint16_t memStart = sizeof(Header) * 12;

    Logger logger;
    Header header;
    Sensors::Measures lastHourStorage[6];
    uint8_t lastHourMeasuresNumber = 0;

    uint8_t putCompactMeasures(CompactMeasures measures);
    CompactMeasures getCompactMeasures(uint16_t);
    CompactMeasures measuresToCompactMeasures(Sensors::Measures);
    uint32_t getCRC(uint16_t);
    Sensors::Measures compactMeasuresToMeasures(CompactMeasures);
    Header getLastHeader();
    uint8_t getCurrentMonthNumber();
    void clear();
    Header computeHeader(uint8_t);
    uint16_t getCurrentHeaderPtr();
};

//Storage storage = Storage::get();