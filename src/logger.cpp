#pragma once

#include "logger.h"

Logger::Logger(const __FlashStringHelper *name, Levels level) : name(name), level(level) {
    Serial.begin(9600);
}

template<typename... Args>
void Logger::error(Args... args) {
    if (!this->level >= ERROR) {
        return;
    }

    this->print('[');
    this->print(this->name);
    this->print(F("] "));
    Serial.print(F("ERROR: "));
    this->print(args...);
    Serial.println();
    Serial.flush();
}

#if DEBUG

template<typename... Args>
void Logger::info(Args... args) {
    if (!this->level >= INFO) {
        return;
    }

    this->print('[');
    this->print(this->name);
    this->print(F("] "));
    Serial.print(F("INFO: "));
    this->print(args...);
    Serial.println();
    Serial.flush();
}

#endif

void Logger::noop() {}

template<typename T, typename... Args>
void Logger::print(T t, Args... args) {
    Serial.print(t);
    Serial.print(' ');
    this->print(args...);
}

template<typename T>
void Logger::print(T t) {
    Serial.print(t);
}