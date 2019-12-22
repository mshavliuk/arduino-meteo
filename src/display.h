#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <vector>
#include <array>
#include <inttypes.h>

#include "logger.h"

#define DISPLAY_ADDR 0x27
#define SPACE 32
#define FILL 255
#define LOG_LEVEL Logger::ERROR
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))


typedef std::vector<std::vector<uint8_t>> byte_matrix;

typedef const uint8_t char_tile[8];


class Display {
public:
    enum modes {
        BIG,
        NORMAL
    };

    Display();

    uint8_t write(const char *string, uint8_t x, uint8_t y, modes mode = NORMAL);
private:
    static const char_tile bigCharBasicTiles[];

    LiquidCrystal_I2C lcd;
    Logger logger;

    uint8_t writeBig(const char *string, uint8_t x, uint8_t y);
    void setBigDigits();
    uint8_t drawByBytesMatrix(uint8_t x, uint8_t y, byte_matrix& charBytes);
    byte_matrix getBigCharByteMatrix(char character);
    uint8_t writeBigDigit(char character, uint8_t x, uint8_t y);
    // TODO: benchmark
};