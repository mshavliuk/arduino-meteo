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
    Sensors(): mhz19(MHZ_TX, MHZ_RX), bme(), timer(MS, 5000) {
        Serial.print("Sensors setup");
        this->mhz19.setAutoCalibration(false);

        this->bme.begin(&Wire);
        this->bme.setSampling(Adafruit_BME280::MODE_FORCED,
                     Adafruit_BME280::SAMPLING_X16, // temperature
                     Adafruit_BME280::SAMPLING_X16, // pressure
                     Adafruit_BME280::SAMPLING_X16, // humidity
                     Adafruit_BME280::FILTER_OFF);
        /*    if (! bme.begin(&Wire)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }*/
    }

    boolean isReady() {
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

            Serial.print("MHZ_status: ");
            Serial.println(data.status);
            Serial.print("CO2: ");
            Serial.println(data.co2);
            Serial.print("MHZ_temp: ");
            Serial.println(data.temp);


            this->bme.takeForcedMeasurement();
            Serial.print("BME temp: ");
            Serial.println(bme.readTemperature());
            Serial.print("BME hum: ");
            Serial.println(bme.readHumidity());
            Serial.print("BME press: ");
            Serial.println(int(bme.readPressure() * PASCAL_TO_MM_HG));
        }
    }

    void getCurrentMeasures() {
    }


private:
    GTimer timer; // TODO: replace with defined value
    MHZ19_uart mhz19;
    Adafruit_BME280 bme;
    boolean ready = false;
};