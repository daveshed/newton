#ifndef SENSORSTUB_H
#define SENSORSTUB_H
#include "ArduinoUtils.h"
#include "node.h"
class FakeForceSensor : public Newton::Sensor
{
public:
    FakeForceSensor();
    void begin(void) override;
    int32_t raw_data(void) const override;
    void update(void) override;
    void raw_data(int32_t value);

private:
    int32_t new_data_ = 0L;
    bool updated = false;
};
#endif // SENSORSTUB_H