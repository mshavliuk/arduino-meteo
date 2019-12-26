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
#define LOG_LEVEL Logger::INFO
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define TILE_WIDTH 6
#define TILE_HEIGHT 8
#define MAX_TILES_NUMBER 8


typedef std::vector<std::vector<uint8_t>> byte_matrix;

typedef const uint8_t char_tile[TILE_HEIGHT];


class Display {
public:
    enum modes {
        BIG,
        NORMAL
    };

    Display();

    uint8_t write(const char *string, uint8_t x, uint8_t y, modes mode = NORMAL);

    template<typename T>
    void writeGraphTile(T *values, uint8_t number, uint8_t x, uint8_t y);
private:
    static const char_tile bigCharBasicTiles[];
    uint8_t activeTilesNumber = 0;

    LiquidCrystal_I2C lcd;
    Logger logger;

    uint8_t writeBig(const char *string, uint8_t x, uint8_t y);
    void setBigDigits();
    uint8_t drawByBytesMatrix(uint8_t x, uint8_t y, byte_matrix& charBytes);
    byte_matrix getBigCharByteMatrix(char character);
    uint8_t writeBigDigit(char character, uint8_t x, uint8_t y);
    uint8_t addTile(char_tile);
    // TODO: benchmark
};

#include "display.tcc"