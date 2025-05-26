#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

constexpr double MAX_SAFE_TEMP = 170.0;

class TemperatureSensor {
  public:
    virtual float read();
    virtual bool hasError();
    virtual void setup();
};

#endif // TEMPERATURESENSOR_H
