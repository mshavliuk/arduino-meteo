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
#define TILE_WIDTH 5
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

    Display(Display const &) = delete;
    static Display &get();
    void clear();
    void operator=(Display const &) = delete;
    uint8_t write(const char *, uint8_t, uint8_t, modes = NORMAL);

    template<typename T>
    void writeGraphTile(T *values, uint8_t, uint8_t);
private:
    Display();
    static const char_tile bigCharBasicTiles[];
    uint8_t activeTilesNumber = 0;

    LiquidCrystal_I2C lcd;
    Logger *logger;

    uint8_t writeBig(const char *, uint8_t, uint8_t);
    void setBigDigits();
    uint8_t drawByBytesMatrix(uint8_t, uint8_t, byte_matrix&);
    byte_matrix getBigCharByteMatrix(char);
    uint8_t writeBigDigit(char, uint8_t, uint8_t);
    uint8_t addTile(char_tile);
    // TODO: benchmark
};

#include "display.tcc"