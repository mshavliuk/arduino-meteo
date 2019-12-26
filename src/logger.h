#pragma once

#include <Arduino.h>
#include <Wire.h>

#if !DEBUG
#define info(...) noop()
#define printFreeMem() noop()
#define printMemoryDump() noop()
#endif

class Logger {
public:
    enum Levels {
        ERROR,
        INFO,
    };

    explicit Logger(const __FlashStringHelper *name, Levels level = INFO);

    template<typename... Args>
    void error(Args... args) {
        if (this->level < ERROR) {
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
    void info(Args... args) {
        if (this->level < INFO) {
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
    void printFreeMem();
    void printMemoryDump();
#endif

    void noop();

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