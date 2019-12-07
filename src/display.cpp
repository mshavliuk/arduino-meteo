#pragma once
#include <LiquidCrystal_I2C.h>
#include <vector>
#include <array>
#include <set>

#include "logger.cpp"

#define DISPLAY_ADDR 0x27
#define SPACE 32
#define FILL 255

typedef std::vector<std::vector<uint8_t>> byte_matrix;
typedef std::array<uint8_t, 8> char_tile;

uint8_t LT[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F};
uint8_t UB[8] = {0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t UMB[8] = {0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F};
uint8_t LMB[8] = {0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F};
uint8_t LM2[8] = {0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t LB[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F};


const char_tile basicChars[] = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F},
        {0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F},
        {0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F},
        {0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F}
};
const char_tile fillTile = {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};


class Display {
public:
    Display() : lcd(DISPLAY_ADDR, 20, 4), logger("Display") {
        this->logger.info("init display");
        this->lcd.init();
        this->lcd.backlight();
    }

    enum modes {
        BIG,
        NORMAL
    };

    void write(char *string, uint8_t x, uint8_t y, modes mode = NORMAL) {
        if (mode == NORMAL) {
            this->lcd.setCursor(x, y);
            this->lcd.println(string);
        } else if (mode == BIG) {
            this->setBigDigits();

            this->logger.info("Writing big chars:", string);
            uint8_t shift = 0;
            for (uint8_t cur = 0; string[cur] != '\0'; cur++) {
                shift += this->writeBigDigit(string[cur], x + shift, y);
//                delay(1000);
            }
        }
    }

private:
    LiquidCrystal_I2C lcd;
    Logger logger;

    struct TileComparator {
        bool operator() (const char_tile& a, const char_tile& b) const {
            for(auto i = 0; i < (uint8_t) sizeof(char_tile); i++) {
                if(a.at(i) < b.at(i)) {
                    return true;
                }
            }
            return false;
        }
    };


    void getExtraCharSet() {}
    
    void setBigDigits() {
        this->lcd.createChar(0, LT);
        this->lcd.createChar(1, UB);
        this->lcd.createChar(2, LB);
        this->lcd.createChar(3, UMB);
        this->lcd.createChar(4, LMB);
        this->lcd.createChar(5, LM2);
        this->lcd.createChar(6, LM2);
    }

    uint8_t drawByBytesMatrix(uint8_t x, uint8_t y, byte_matrix& charBytes) {
        uint8_t width = 0;
        for(auto& row : charBytes) {
            lcd.setCursor(x, y);
            for(auto& charByte : row) {
                lcd.write(charByte);
            }
            width = width > row.size() ? width : row.size();
            y++;
        }
        return width;
    }

    void clearSectors(uint8_t x, uint8_t y, uint8_t height, uint8_t width) {
        for (auto j = y; j < y + height; j++) {
            lcd.setCursor(x, j);
            for (auto i = 0; i < width; i++) {
                lcd.write(' ');
            }
        }
    }

    std::set<char_tile, TileComparator> makeBigCharThinner(byte_matrix& matrix) {
        std::set<char_tile, TileComparator> extraChars;

        auto applyBitMask = [](const char_tile charTile, uint8_t mask){
            char_tile result;
            for(uint8_t i = 0; i < (uint8_t) sizeof(char_tile); i++) {
//                result[i] = charTile.at(i) & mask;
                result[i] = charTile[i];
            }
            return result;
        };

        for(auto& row : matrix) {
            for(size_t i = 0; i < row.size(); i++) {
                char_tile tile;
                if (row[i] == FILL) {
                    tile = fillTile;
                } else {
                    tile = basicChars[row[i]];
                }

                if (row[i] > sizeof(basicChars) && row[i] != FILL) {
                    continue;
                } else if (i == row.size() - 1 || row[i + 1] == SPACE) {
                    extraChars.insert(applyBitMask(tile, 0x1Eu));
                } else if (i == 0 || row[i - 1] == SPACE) {
                    extraChars.insert(applyBitMask(tile, 0x0Fu));
                } else {
                    extraChars.insert(tile);
                }
                row[i] = extraChars.size() - 1;
            }
        }

        return extraChars;
    }

    byte_matrix getBigCharByteMatrix(char character) {
        switch (character) {
            case '0':
                return {{FILL, 1, FILL},
                        {FILL, 2, FILL}};
            case '1':
                return {{SPACE, FILL, SPACE},
                        {SPACE, FILL, SPACE}};
            case '2':
                return {{3,   3, FILL},
                        {FILL, 4, 4}};
            case '3':
                return {{3, 3, FILL},
                        {4, 4, FILL}};
            case '4':
                return {{FILL, 0, FILL},
                        {5,   5, FILL}};
            case '5':
                return {{FILL, 3, 3},
                        {4,   4, FILL}};
            case '6':
                return {{FILL, 3, 3},
                        {FILL, 4, FILL}};
            case '7':
                return {{1,  1,   FILL},
                        {SPACE, FILL, SPACE}};
            case '8':
                return {{FILL, 3, FILL},
                        {FILL, 4, FILL}};
            case '9':
                return {{FILL, 3, FILL},
                        {4,   4, FILL}};
            case ':':
                return {{165},
                        {165}};
            case ' ':
                return {{SPACE},
                        {SPACE}};
            default:
                this->logger.error("Big char is not supported:", character);
                return {{63},
                        {63}};
        }
    }

    void setExtraTiles(const std::set<char_tile, TileComparator>& tiles) {
//        for(auto tile : tiles) {
//            for(auto row : tile) {
//              Serial.print(row, HEX);
//                Serial.print(' ');
//            }
//            Serial.println(' ');
//        }
        uint8_t n = 0;
        for (auto tile : tiles) {
            Serial.print("new char #");
            Serial.print(n, DEC);
            Serial.print(": ");
            char tileData[8];
            for(uint8_t i = 0; i < (uint8_t) tile.size(); i++) {
                Serial.print(tile[i], HEX);
                Serial.print(" ");
                tileData[i] = (char) tile[i];
            }


            this->lcd.createChar(n++, tile.data());
            Serial.println("");
        }
    }

    uint8_t writeBigDigit(char character, uint8_t x, uint8_t y) {
        auto charBytes = this->getBigCharByteMatrix(character);

//        auto tiles = this->makeBigCharThinner(charBytes);
//        this->makeBigCharThinner(charBytes);

//        Serial.println('Char thinner tile: ');
//        for(auto tile : tiles) {
//            for(auto row : tile) {
//              Serial.print(row, HEX);
//                Serial.print(' ');
//            }
//            Serial.println(' ');
//        }
//        this->setExtraTiles(tiles);
        uint8_t width = this->drawByBytesMatrix(x, y, charBytes);
        return width;
    }
};