#pragma once

#include <Arduino.h>
#include <Wire.h>

#if !DEBUG
#define info(...) noop()
#endif

class Logger {
public:
    enum Levels {
        ERROR,
        INFO,
    };

    Logger(const __FlashStringHelper *name, Levels level = INFO) : name(name), level(level) {
        Serial.begin(9600);
    }

    template<typename... Args>
    void error(Args... args) {
        if(!this->level >= ERROR) {
            return;
        }

        this->print('[');
        this->print(this->name);
        this->print(F("] "));
        Serial.print(F("ERROR: "));
        this->print(args...);
        Serial.print('\n');
        Serial.flush();
    }

#if DEBUG
    template<typename... Args>
    void info(Args... args) {
        if(!this->level >= INFO) {
            return;
        }

        this->print('[');
        this->print(this->name);
        this->print(F("] "));
        Serial.print(F("INFO: "));
        this->print(args...);
        Serial.print('\n');
        Serial.flush();
    }
#endif

    void noop() {}

private:
    const __FlashStringHelper *name;
    Levels level;

    template<typename T, typename... Args>
    void print(T t, Args... args) {
        Serial.print(t);
        Serial.print(' ');
        this->print(args...);
    }

    template<typename T>
    void print(T t) {
        Serial.print(t);
    }
};