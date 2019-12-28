
#include "MainPage.h"


MainPage::MainPage() : Page(F("MainPage")), timer(MS, 1000), displayDots(true) {}


MainPage::~MainPage() {}

void MainPage::draw() {
    this->logger->info(F("draw MainPage"));

    if (!Sensors::get().isMeasuresReady()) {
        Display::get().write("Waiting for sensors", 0, 1);
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
    Display::get().write(timeString, 0, 0, Display::BIG);
    char dateString[6] = "  /  ";
    itoa(now.day(), 2, dateString, true);
    itoa(now.month(), 2, dateString + 3, true);
    Display::get().write(dateString, 15, 0);

    Display::get().write("    ", 15, 1); // TODO: remove
    Display::get().write(this->weekDays[now.dayOfTheWeek()], 15, 1);

    Measures measures = Sensors::get().getCurrentMeasures();
    char tempString[] = "99.9\xDF";
    this->ftoa(measures.temp, 4, 1, tempString);
    Display::get().write(tempString, 6, 2);

    char ppmString[] = "    ppm";
    this->itoa(measures.ppm, PPM_WIDTH, ppmString, false);
    Display::get().write(ppmString, 4, 3);

    char humidityString[] = "  %";
    itoa((uint8_t) measures.humidity, HUMID_WIDTH, humidityString, false);
    Display::get().write(humidityString, 0, 3);

    char pressureString[] = "   mm";
    itoa(measures.pressHg, PRESS_WIDTH, pressureString, false);
    Display::get().write(pressureString, 0, 2);

    uint16_t ppmArray[TEMP_MEASURES_NUMBER];
    float tempArray[TEMP_MEASURES_NUMBER];
    Measures *m = Storage::getInstance().getTempMeasures();
    for (uint8_t i = 0; i < TEMP_MEASURES_NUMBER; ++i) {
        ppmArray[i] = m[i].ppm;
        tempArray[i] = m[i].temp;
    }
    Display::get().writeGraphTile(ppmArray, 12, 3);
    Display::get().writeGraphTile(tempArray, 12, 2);
}


bool MainPage::tick() {
    if (this->timer.isReady()) {
        this->displayDots = !this->displayDots;
        this->draw();
        return true;
    }
    return false;
}

char *MainPage::itoa(uint16_t n, uint8_t numberMaxWidth, char *buf, bool leadZero) {
    for (uint8_t i = 0; i < numberMaxWidth; ++i) {
        uint8_t digit = (n / (uint16_t) this->ipow(10, numberMaxWidth - i - 1)) % 10;
        if (!leadZero && digit == 0) {
            buf[i] = ' ';
        } else {
            buf[i] = static_cast<char>(digit + '0');
            leadZero = true;
        }
    }

    return buf;
}

char *MainPage::ftoa(double n, uint8_t width, uint8_t precision, char *buf) {
    this->itoa(n, width - precision - 1, buf, false);
    buf[width - precision - 1] = '.';
    double fraction = modf(n, &fraction);
    this->itoa(fraction * ipow(10, precision), precision, &buf[width - precision], true);
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
