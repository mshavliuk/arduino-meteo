#pragma once

#include <Arduino.h>
#include <GyverTimer.h>
#include <MHZ19_uart.h>
#include <Adafruit_BME280.h>


#define MHZ_RX 2
#define MHZ_TX 3

#define PASCAL_TO_MM_HG 0.00750062


class Sensors {
public:
    Sensors(): mhz19(MHZ_TX, MHZ_RX), bme(), timer(MS, 5000), logger("Sensors") {
        this->logger.info("Starting setup");
        this->mhz19.setAutoCalibration(false);

        if (!this->bme.begin(&Wire)) {
            this->logger.error("Could not find a valid BME280 sensor, check wiring!");
            while (1);
        }
        this->bme.setSampling(Adafruit_BME280::MODE_FORCED,
                     Adafruit_BME280::SAMPLING_X16, // temperature
                     Adafruit_BME280::SAMPLING_X16, // pressure
                     Adafruit_BME280::SAMPLING_X16, // humidity
                     Adafruit_BME280::FILTER_OFF);
        this->logger.info("Setup is finished");
    }

    bool isReady() {
        if(!this->ready) {
            if(mhz19.getStatus() == 0) {
                this->ready = true;
                return true;
            } else {
                return false;
            }
        }

        return true;
    }

    void tick() {
        if(this->timer.isReady() && this->isReady()) {
            mhz_data data = mhz19.getData();

            this->logger.info("MHZ_status:", data.status);
            this->logger.info("CO2:", data.co2);
            this->logger.info("MHZ_temp:", data.temp);


            this->bme.takeForcedMeasurement();
            this->logger.info("BME temp:", bme.readTemperature());
            this->logger.info("BME hum:", bme.readHumidity());
            this->logger.info("BME press:", int(bme.readPressure() * PASCAL_TO_MM_HG));
        }
    }

    void getCurrentMeasures() {
    }


private:
    Logger logger;
    GTimer timer;
    MHZ19_uart mhz19;
    Adafruit_BME280 bme;
    bool ready = false;
};