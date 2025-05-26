#ifndef MAX6675THERMOCOUPLE_H
#define MAX6675THERMOCOUPLE_H

#include "TemperatureSensor.h"
#include <MAX6675.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

constexpr int MAX6675_UPDATE_INTERVAL = 250;
constexpr int MAX6675_MAX_ERRORS = 20;

using temperature_callback_t = std::function<void(float)>;
using temperature_error_callback_t = std::function<void()>;

class Max6675Thermocouple : public TemperatureSensor {
  public:
  Max6675Thermocouple(int csPin, int misoPin, int sckPin, const temperature_callback_t &callback,
                         const temperature_error_callback_t &error_callback);
    float read() override;
    bool hasError() override;

    void setup() override;
    void loop();

  private:
    MAX6675 *max6675;
    xTaskHandle taskHandle;

    int errors = 0;
    float temperature = .0f;

    int csPin = 0;
    int misoPin = 0;
    int sckPin = 0;

    temperature_callback_t callback;
    temperature_error_callback_t error_callback;

    const char *LOG_TAG = "MAX6675Thermocouple";
    static void monitorTask(void *arg);
};

#endif // MAX6675THERMOCOUPLE_H
