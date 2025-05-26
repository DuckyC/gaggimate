#include "main.h"
#include "DimmedPump.h"

DimmedPump *pump;

void setup() {
    Serial.begin(115200);
    pump = new DimmedPump(11, 7);
    pump->setPower(0.5f);
}

void loop() {
}
