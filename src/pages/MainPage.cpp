
#include "MainPage.h"


MainPage::MainPage(Sensors *sensors) : Page(F("MainPage")), sensors(sensors), timer(MS, 500), displayDots(true) {}

void MainPage::draw(Display *display) {
    this->logger.info(F("draw MainPage"));

    if (!sensors->isMeasuresReady()) {
        display->write("Waiting for sensors", 0, 1);
        return;
    }

    Wire.begin(); // added to prevent this issue https://forum.arduino.cc/index.php?topic=124286.0
    DateTime now = RTC_DS3231::now();

    char timeString[] = {
            static_cast<char>(now.hour() / 10 + '0'),
            ' ',
            static_cast<char>(now.hour() % 10 + '0'),
            this->displayDots ? ':' : ' ',
            static_cast<char>(now.minute() / 10 + '0'),
            ' ',
            static_cast<char>(now.minute() % 10 + '0'),
            '\0'
    };
    display->write(timeString, 0, 0, Display::BIG);
    // TODO: display date
    // TODO: display day of the week

    Measures measures = this->sensors->getCurrentMeasures();
    char tempString[TEMP_WIDTH + sizeof("\xDF")];
    this->ftoa(measures.temp, 4, 1, tempString, "\xDF");
    display->write(tempString, 6, 2);

    char ppmString[PPM_WIDTH + sizeof("ppm")];
    this->itoa(measures.ppm, 4, ppmString, "ppm");
    display->write(ppmString, 4, 3);

    char humidityString[HUMID_WIDTH + sizeof("%")];
    itoa((uint8_t) measures.humidity, HUMID_WIDTH, humidityString, "%");
    display->write(humidityString, 0, 3);

    char pressureString[PRESS_WIDTH + sizeof("mm")];
    itoa(measures.pressHg, PRESS_WIDTH, pressureString, "mm");
    display->write(pressureString, 0, 2);

    uint16_t ppmArray[6];
    float tempArray[6];
    Measures *m = Storage::getInstance().getTempMeasures();
    for(uint8_t i = 0; i < 6; ++i) {
        ppmArray[i] = m[i].ppm;
        tempArray[i] = m[i].temp;
    }
    display->writeGraphTile(ppmArray, 6, 12, 3);
    display->writeGraphTile(tempArray, 6, 12, 2);
}


bool MainPage::tick(Display *display) {
    if (this->timer.isReady()) {
        this->displayDots = !this->displayDots;
        this->draw(display);
        return true;
    }
    return false;
}

char* MainPage::itoa(uint16_t n, uint8_t numberMaxWidth, char *buf, const char *postfix, bool leadZero) {
    for (uint8_t i = 0; i < numberMaxWidth; ++i) {
        uint8_t digit = (n / (uint16_t) this->ipow(10, numberMaxWidth - i - 1)) % 10;
        if (!leadZero && digit == 0) {
            buf[i] = ' ';
        } else {
            buf[i] = static_cast<char>(digit + '0');
            leadZero = true;
        }
    }

    uint8_t i;
    for (i = 0; postfix[i] != '\0'; ++i) {
        buf[numberMaxWidth + i] = postfix[i];
    }

    buf[numberMaxWidth + i] = '\0';
    return buf;
}

char* MainPage::ftoa(double n, uint8_t width, uint8_t precision, char *buf, const char *postfix) {
    this->itoa(n, width - precision - 1, buf, ".");
    double fraction = modf(n, &fraction);
    this->itoa(fraction * ipow(10, precision), precision, &buf[width - precision], postfix, true);
    return buf;
}

uint16_t MainPage::ipow(int16_t base, uint8_t exponent) {
    if (exponent == 0) {
        return 1;
    }
    int16_t res = base;
    for (auto i = 1; i < exponent; ++i) {
        res *= base;
    }
    return res;
}