#pragma once

#include <Arduino.h>



class Logger {
public:
    Logger(): Logger("") {
    }


    Logger(char* name): name(name), hasName(name[0] != '\0') {
        Serial.begin(9600);
    }

    template<typename... Args>
    void error(Args... args)
    {
        Serial.print("ERROR: ");
        this->print(args...);
        Serial.print('\n');
        Serial.flush();
    }

    template<typename... Args>
    void info(Args... args)
    {
#if DEBUG
        if(this->hasName) {
            this->print('[');
            this->print(this->name);
            this->print("] ");
        }
        this->print(args...);
        Serial.print('\n');
        Serial.flush();
#endif
    }

private:
    bool hasName;
    char* name;

    template<typename T, typename... Args>
    void print(T t, Args... args) {
        Serial.print(t);
        Serial.print(' ');
        this->print(args...);
    }
    template <typename T>
    void print(T t) {
        Serial.print(t);
    }
};