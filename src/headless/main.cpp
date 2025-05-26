#include "main.h"

Controller controller;

void setup() {
    Serial.begin(115200);
    controller.setup();
}

void loop() {
    controller.loop();
    delay(2);
}
