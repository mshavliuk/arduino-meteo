
template<typename T>
void Display::writeGraphTile(T *values, uint8_t number, uint8_t x, uint8_t y) {
    if (number == 0) {
        return;
    }

    T min = values[1];
    T max = values[1];

    for (auto i = 1; i < TILE_WIDTH; ++i) {
        min = min < values[i] ? min : values[i];
        max = max > values[i] ? max : values[i];
    }

    this->logger.info(F("min:"), min, F("max:"), max);

    uint8_t tile[TILE_HEIGHT];
    for (uint8_t i = 0; i < TILE_HEIGHT - 1; ++i) {    // 8 is height of one tile
        uint8_t row = 0;
        for (uint8_t j = 0; j < TILE_WIDTH; ++j) {    // 6 is width of one tile
            row |= (((values[j] - min) * (TILE_HEIGHT - 1.0F) / float(max - min)) > i) << (TILE_WIDTH - 1 - j);
        }
        tile[TILE_HEIGHT - 1 - i] = row;
    }
    tile[TILE_HEIGHT - 1] = 0x1FU;

    auto tileNumber = this->addTile(tile);
    lcd.setCursor(x, y);
    lcd.write(tileNumber);
}