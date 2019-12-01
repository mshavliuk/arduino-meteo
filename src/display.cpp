#pragma once
#include <LiquidCrystal_I2C.h>
#include <vector>

#define DISPLAY_ADDR 0x27

uint8_t LT[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
uint8_t UB[8] = {0b11111, 0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000,
                 0b00000};   // для двустрочных 7, 0   // для четырехстрочных 2, 3, 4, 5, 6, 8, 9
uint8_t UMB[8] = {0b11111, 0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b11111,
                  0b11111};  // для двустрочных 2, 3, 5, 6, 8, 9
uint8_t LMB[8] = {0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111,
                  0b11111};  // для двустрочных 2, 3, 5, 6, 8, 9
uint8_t LM2[8] = {0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};  // для двустрочной 4
uint8_t LB[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111};   // для 0


class Display {
public:
    Display() : lcd(DISPLAY_ADDR, 20, 4) {
        Serial.println("init display");
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

            Serial.println(string);
            for (uint8_t cur = 0; string[cur] != '\0'; cur++) {
                this->writeBigDigit(string[cur], x + cur * 4, y);
            }
        }
    }

private:
    LiquidCrystal_I2C lcd;

    void setBigDigits() {
        this->lcd.createChar(0, LT);
        this->lcd.createChar(1, UB);
        this->lcd.createChar(2, LB);
        this->lcd.createChar(3, UMB);
        this->lcd.createChar(4, LMB);
        this->lcd.createChar(5, LM2);
    }

    void digSeg(byte x, byte y, std::vector<std::vector<byte>>& charBytes) {
        for(auto& row : charBytes) {
            lcd.setCursor(x, y);
            for(auto& charByte : row) {
                lcd.write(charByte);
            }
            lcd.write(32); // only for x > 11
            y++;
        }
//        if (x <= 11) {
//            lcd.print(" ");
//        }
    }

    void writeBigDigit(char dig, byte x, byte y) {
        std::vector<std::vector<byte>> charBytes;
        switch (dig) {
            case '0':
                charBytes = {{255, 1, 255}, {255, 2, 255}};
                this->digSeg(x, y, charBytes);
                break;
            case '1':
                charBytes = {{32, 255, 32}, {32, 255, 32}};
                this->digSeg(x, y, charBytes);
                break;
            case '2':
                charBytes = {{3, 3, 255}, {255, 4, 4}};
                this->digSeg(x, y, charBytes);
                break;
            case '3':
                charBytes = {{3, 3, 255}, {4, 4, 255}};
                this->digSeg(x, y, charBytes);
                break;
            case '4':
                charBytes = {{255, 0, 255}, {5, 5, 255}};
                this->digSeg(x, y, charBytes);
                break;
            case '5':
                charBytes = {{255, 3, 3}, {4, 4, 255}};
                this->digSeg(x, y, charBytes);
                break;
            case '6':
                charBytes = {{255, 3, 3}, {255, 4, 255}};
                this->digSeg(x, y, charBytes);
                break;
            case '7':
                charBytes = {{1, 1, 255}, {32, 255, 32}};
                this->digSeg(x, y, charBytes);
                break;
            case '8':
                charBytes = {{255, 3, 255}, {255, 4, 255}};
                this->digSeg(x, y, charBytes);
                break;
            case '9':
                charBytes = {{255, 3, 255}, {4, 4, 255}};
                this->digSeg(x, y, charBytes);
                break;
            default:
                Serial.println("Big char is not supported");
                Serial.println(dig);
        }
    }
};