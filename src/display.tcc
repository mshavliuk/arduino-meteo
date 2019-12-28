
template<typename T>
void Display::writeGraphTile(T *values, uint8_t x, uint8_t y) {
    T min = values[0];
    T max = values[0];

    for (auto i = 1; i < TILE_WIDTH; ++i) {
        min = min < values[i] ? min : values[i];
        max = max > values[i] ? max : values[i];
    }

    this->logger->info(F("min:"), min, F("max:"), max);

    uint8_t tile[TILE_HEIGHT];
    for (uint8_t i = 0; i < TILE_HEIGHT; ++i) {
        uint8_t row = 0;
        for (uint8_t j = 0; j < TILE_WIDTH; ++j) {
            row |= (((values[j] - min) * (TILE_HEIGHT - 1.0F) / float(max - min)) > (i - 1)) << (TILE_WIDTH - 1 - j);
        }
        tile[TILE_HEIGHT - 1 - i] = row;
        this->logger->info(F("Tile row #"), TILE_HEIGHT - 1 - i, row);
    }

    auto tileNumber = this->addTile(tile);
    lcd.setCursor(x, y);
    lcd.write(tileNumber);
}