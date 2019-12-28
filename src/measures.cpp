
#include "measures.h"

Measures::Measures(uint16_t ppm, float temp, uint8_t humidity, uint16_t press)
        : ppm(ppm), temp(temp), humidity(humidity), pressHg(press) {}

#if DEBUG

size_t Measures::printTo(Print &p) const {
    size_t size = 0;

    size += p.print(F("Measures(ppm: "));
    size += p.print(this->ppm);
    size += p.print(F("; temp: "));
    size += p.print(this->temp);
    size += p.print(F("; humidity: "));
    size += p.print(this->humidity);
    size += p.print(F("; pressure: "));
    size += p.print(this->pressHg);
    size += p.print(F(")"));

    return size;
}

#else
size_t Measures::printTo(Print &p) const {
    return 0;
}
#endif
