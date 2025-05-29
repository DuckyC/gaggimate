#include "main.h"

Controller *controller;

void setup() {
    Serial.begin(115200);
    controller = new Controller();
#ifdef ENABLE_DISPLAY
    auto ui = new DefaultUI(controller, controller->getPluginManager());
#endif
    controller->setup();
#ifdef ENABLE_DISPLAY
    ui->init();
#else
    controller->onScreenReady();
#endif
}

void loop() {
    controller->loop();
    delay(2);
}
