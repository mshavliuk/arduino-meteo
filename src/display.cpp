
#include "display.h"

Display::Display() : lcd(DISPLAY_ADDR, 20, 4), logger(F("Display"), LOG_LEVEL) {
    this->logger.info(F("init display"));
    this->lcd.init();
    this->lcd.backlight();
    this->lcd.cursor_off();
    this->lcd.blink_off();
}

uint8_t Display::write(const char *string, const uint8_t x, const uint8_t y, const modes mode) {
    this->logger.info(F("Writing string:"), string);

    if (mode == NORMAL) {
        this->lcd.setCursor(x, y);
        this->lcd.print(string);
    } else if (mode == BIG) {
        this->setBigDigits();

        return this->writeBig(string, x, y);
    }
    return 0;
}


uint8_t Display::writeBig(const char *string, const uint8_t x, const uint8_t y) {
    uint8_t shift = 0;
    for (uint8_t cur = 0; string[cur] != '\0'; cur++) {
        shift += this->writeBigDigit(string[cur], x + shift, y);
    }
    return shift;
}

const char_tile Display::bigCharBasicTiles[] = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F},
        {0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F},
};

uint8_t Display::drawByBytesMatrix(uint8_t x, uint8_t y, byte_matrix &charBytes) {
    uint8_t width = 0;
    for (auto &row : charBytes) {
        lcd.setCursor(x, y);
        for (auto &charByte : row) {
            lcd.write(charByte);
        }
        width = width > row.size() ? width : row.size();
        y++;
    }
    return width;
}

byte_matrix Display::getBigCharByteMatrix(const char character) {
    switch (character) {
        case '0':
            return {{FILL, 1, FILL},
                    {FILL, 0, FILL}};
        case '1':
            return {{SPACE, FILL, SPACE},
                    {SPACE, FILL, SPACE}};
        case '2':
            return {{2,    2, FILL},
                    {FILL, 2, 2}};
        case '3':
            return {{2, 2, FILL},
                    {2, 2, FILL}};
        case '4':
            return {{FILL, 0, FILL},
                    {1,    1, FILL}};
        case '5':
            return {{FILL, 2, 2},
                    {2,    2, FILL}};
        case '6':
            return {{FILL, 2, 2},
                    {FILL, 2, FILL}};
        case '7':
            return {{1, 1,        FILL},
                    {SPACE, FILL, SPACE}};
        case '8':
            return {{FILL, 2, FILL},
                    {FILL, 2, FILL}};
        case '9':
            return {{FILL, 2, FILL},
                    {2,    2, FILL}};
        case ':':
            return {{165},
                    {165}};
        case ' ':
            return {{SPACE},
                    {SPACE}};
        default:
            this->logger.error(F("Big char is not supported:"), character);
            return {{63},
                    {63}};
    }
}

uint8_t Display::writeBigDigit(const char character, const uint8_t x, const uint8_t y) {
    auto charBytes = this->getBigCharByteMatrix(character);
    uint8_t width = this->drawByBytesMatrix(x, y, charBytes);
    return width;
}

void Display::setBigDigits() {
    auto bigCharTilesNumber = (uint8_t) ARRAY_LENGTH(Display::bigCharBasicTiles);
    for (auto i = 0; i < bigCharTilesNumber; i++) {
        this->lcd.createChar(i, const_cast<uint8_t *>(Display::bigCharBasicTiles[i]));
    }
    this->activeTilesNumber = bigCharTilesNumber;
}

uint8_t Display::addTile(char_tile tile) {
    if(this->activeTilesNumber == MAX_TILES_NUMBER) {
        this->logger.error(F("Max active tiles number reached"));
        return 0;
    }
    this->logger.info(F("Add tile"), *tile);
    this->activeTilesNumber++;
    this->lcd.createChar(this->activeTilesNumber, const_cast<uint8_t *>(tile));
    return this->activeTilesNumber;
}
