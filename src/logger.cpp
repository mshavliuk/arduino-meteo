#pragma once

#include "logger.h"

#include <MemoryFree.h>

Logger::Logger(const __FlashStringHelper *name, Levels level) : name(name), level(level) {
//    Serial.begin(9600);
}



#if DEBUG
void Logger::printFreeMem() {
    this->print(F("Free mem:"), freeMemory());
    Serial.println();
    Serial.flush();
}

void Logger::printMemoryDump() {
    this->printFreeMem();
    uint8_t *ptr = 0;
    Serial.println(F("Memory dump:"));
    for(uint16_t i = 0; i < 2048; ++i) {
        Serial.print(ptr[i], HEX);
        Serial.print(' ');
        if(i != 0 && i % 8 == 0) {
            Serial.println(' ');
        }
    }
    Serial.println(' ');
    Serial.println(F("-------------END--------------"));
}
#endif

void Logger::noop() {}

