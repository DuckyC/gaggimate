#include "Max6675Thermocouple.h"
#include <Arduino.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>

Max6675Thermocouple::Max6675Thermocouple(const int csPin, const int misoPin, const int sckPin,
                                           const temperature_callback_t &callback,
                                           const temperature_error_callback_t &error_callback)
    : taskHandle(nullptr), csPin(csPin), misoPin(misoPin), sckPin(sckPin) {
    max6675 = new MAX6675(csPin, misoPin, sckPin);
    this->callback = callback;
    this->error_callback = error_callback;
}

float Max6675Thermocouple::read() { return temperature; }

bool Max6675Thermocouple::hasError() { return errors >= MAX6675_MAX_ERRORS; }

void Max6675Thermocouple::setup() {
    SPI.begin();
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);
    max6675->begin();
    max6675->setSPIspeed(1000000);

    xTaskCreate(monitorTask, "MAX6675Thermocouple::monitor", configMINIMAL_STACK_SIZE * 4, this, 1, &taskHandle);
}

void Max6675Thermocouple::loop() {
    int status = max6675->read();
    if (status != STATUS_OK) {
        ESP_LOGE(LOG_TAG, "Failed to read temperature: %d\n", status);
        errors++;
    }
    errors = 0;
    float temp = max6675->getTemperature();
    if (temp > 0) {
        temperature = 0.2 * temp + 0.8 * temperature;
    } else {
        errors++;
    }
    if (errors >= MAX6675_MAX_ERRORS || temperature > MAX_SAFE_TEMP) {
        error_callback();
        return;
    }
    ESP_LOGV(LOG_TAG, "Updated temperature: %2f\n", temperature);
    callback(temperature);
}

[[noreturn]] void Max6675Thermocouple::monitorTask(void *arg) {
    auto *thermocouple = static_cast<Max6675Thermocouple *>(arg);
    while (true) {
        thermocouple->loop();
        vTaskDelay(MAX6675_UPDATE_INTERVAL / portTICK_PERIOD_MS);
    }
}
