#pragma once

#include <Printable.h>
#include <Print.h>
#include <inttypes.h>

class Measures : public Printable {
public:
    // TODO: remove default values
    explicit Measures(uint16_t ppm = 0, float temp = 0, uint8_t humidity = 0, uint16_t press = 0);

    uint16_t ppm;
    float temp;
    uint8_t humidity;
    uint16_t pressHg;
    size_t printTo(Print &p) const override;
};