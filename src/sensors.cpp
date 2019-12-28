
#include "sensors.h"
#include "logger.h"

Sensors::Sensors() :
        measures({}),
        mhz19(MHZ_TX, MHZ_RX),
        takeMeasuresTimer(MS, TAKE_MEASURES_INTERVAL),
        logger(new Logger(F("Sensors"))) {
    this->logger->info(F("Starting setup"));
    this->mhz19.setAutoCalibration(false);

    if (!this->bme.begin(&Wire)) {
        this->logger->error(F("Could not find a valid BME280 sensor, check wiring!"));
        while (1);  // TODO: display error message on screen
    }

    this->bme.setSampling(Adafruit_BME280::MODE_FORCED,
                          Adafruit_BME280::SAMPLING_X16,
                          Adafruit_BME280::SAMPLING_X16,
                          Adafruit_BME280::SAMPLING_X16,
                          Adafruit_BME280::FILTER_OFF);
    this->logger->info(F("Setup is finished"));
}

bool Sensors::isReady() {
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

bool Sensors::isMeasuresReady() {
    return this->measuresReady;
}

bool Sensors::tick() {
    if (!this->takeMeasuresTimer.isReady() || !this->isReady()) {
        return false;
    }
    this->takeMeasures();
    Storage::getInstance().addMeasures(this->measures);

    return true;
}

void Sensors::takeMeasures() {
    mhz_data mhzData = this->mhz19.getData();

    this->bme.takeForcedMeasurement();
    this->measures = Measures(
            mhzData.co2,
            bme.readTemperature(),
            uint8_t(bme.readHumidity()),
            uint16_t(bme.readPressure() * PASCAL_TO_MM_HG)
    );
    this->logger->info(this->measures);

    this->measuresReady = true;
}

Measures Sensors::getCurrentMeasures() {
    return this->measures;
}

Sensors &Sensors::get() {
    static Sensors instance;
    return instance;
}
