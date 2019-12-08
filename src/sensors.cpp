#pragma once

#include <GyverTimer.h>
#include <MHZ19_uart.h>
#include <Adafruit_BME280.h>
#include <RTClib.h>
#include <Wire.h>


#define MHZ_RX 2
#define MHZ_TX 3

#define RESET_TIME false

#define PASCAL_TO_MM_HG 0.00750062


class Sensors {
public:
    Sensors() : mhz19(MHZ_TX, MHZ_RX), bme(), rtc(), timer(MS, 5000), logger(F("Sensors")) {
        this->logger.info(F("Starting setup"));
        this->mhz19.setAutoCalibration(false);

        if (!this->bme.begin(&Wire)) {
            this->logger.error(F("Could not find a valid BME280 sensor, check wiring!"));
            while (1);  // TODO: display error message on screen
        }

        if (!this->rtc.begin()) {
            this->logger.error(F("Could not set up an RTC module"));
            while (1);  // TODO: display error message on screen
        }

        this->bme.setSampling(Adafruit_BME280::MODE_FORCED,
                              Adafruit_BME280::SAMPLING_X16,
                              Adafruit_BME280::SAMPLING_X16,
                              Adafruit_BME280::SAMPLING_X16,
                              Adafruit_BME280::FILTER_OFF);
        this->logger.info(F("Setup is finished"));

        if (RESET_TIME || this->rtc.lostPower()) {
            this->rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            this->logger.info("Time was adjasted:", F(__DATE__), F(__TIME__));
        }
    }

    bool isReady() {
        if (this->ready) {
            return true;
        }

        if (this->mhz19.getStatus() == 0) {
            this->ready = true;
            return true;
        } else {
            return false;
        }
    }

    bool tick() {
        if (!this->timer.isReady() || !this->isReady()) {
            return false;
        }

        mhz_data mhzData = this->mhz19.getData();

        this->logger.info(F("MHZ_status:"), mhzData.status);
        this->logger.info(F("CO2:"), mhzData.co2);
        this->logger.info(F("MHZ_temp:"), mhzData.temp);

        this->logger.info(F("RTC temp:"), this->rtc.getTemperature());


        this->bme.takeForcedMeasurement();

        this->measures = {mhzData.co2, bme.readTemperature(), bme.readHumidity(),
                          int(bme.readPressure() * PASCAL_TO_MM_HG)};

        this->logger.info(F("BME temp:"), this->measures.temp);
        this->logger.info(F("BME hum:"), this->measures.humidity);
        this->logger.info(F("BME press:"), this->measures.pressHg);

        return true;
    }


    struct Measures {
        uint16_t ppm;
        float temp;
        float humidity;
        uint16_t pressHg;
    };

    Sensors::Measures getCurrentMeasures() {
        return this->measures;
    }

    DateTime getDateTime() {
        return this->rtc.now();
    }

private:
    Measures measures;
    Logger logger;
    GTimer timer;
    MHZ19_uart mhz19;
    Adafruit_BME280 bme;
    RTC_DS3231 rtc;
    bool ready = false;
};