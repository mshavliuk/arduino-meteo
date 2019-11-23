#include <Arduino.h>

#define MHZ_RX 2
#define MHZ_TX 3

#include <Adafruit_BME280.h>
#include <MHZ19_uart.h>
#include <RTClib.h>


MHZ19_uart mhz19;
Adafruit_BME280 bme;
RTC_DS3231 rtc;

float dispTemp;
byte dispHum;
int dispPres;

void readSensors() {
    bme.takeForcedMeasurement();
    dispTemp = bme.readTemperature();
    dispHum = bme.readHumidity();
    dispPres = int(bme.readPressure() * 0.00750062);
}

void writeSensors() {
    Serial.print("temp: ");
    Serial.print(dispTemp);
    Serial.print("\nhum: ");
    Serial.print(dispHum);
    Serial.print("\npress: ");
    Serial.print(dispPres);
}

void helloworld() {
    Serial.print("MHZ-19... ");
    mhz19.begin(MHZ_TX, MHZ_RX);
    mhz19.setAutoCalibration(false);
    mhz19.getStatus();    // первый запрос, в любом случае возвращает -1
    delay(500);
    if (mhz19.getStatus() == 0) {
        Serial.println(F("OK"));
    } else {
        Serial.println(F("ERROR"));
    }
}

void setup() {
    Serial.begin(9600);

    helloworld();

    bool res = bme.begin(&Wire);
    Serial.print("BME begin: ");
    Serial.println(res);

    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1, // temperature
                    Adafruit_BME280::SAMPLING_X1, // pressure
                    Adafruit_BME280::SAMPLING_X1, // humidity
                    Adafruit_BME280::FILTER_OFF   );

    if(rtc.begin()) {
        Serial.println("RTC ok");
    } else {
        Serial.println("RTC fail");
    }
}

void looptest() {
    mhz_data data = mhz19.getData();

    Serial.print("MHZ_status: ");
    Serial.println(data.status);
    Serial.print("CO2: ");
    Serial.println(data.co2);
    Serial.print("BME_temp: ");
    Serial.println(bme.readTemperature());
    Serial.print("MHZ_temp: ");
    Serial.println(data.temp);
    Serial.print("RTC_temp: ");
    Serial.println(rtc.getTemperature());
}

void loop() {
    looptest();
    delay(5000);
}
