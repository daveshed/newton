// real sensor implementation
#ifndef SENSOR_H
#define SENSOR_H
#include "HX711.h"

#include "node.h"

class HX711ForceSensor : public Newton::Sensor {
public:
    HX711ForceSensor();
    void begin(void) override;
    void update(void) override;
    int32_t raw_data(void) const override;

private:
    HX711 device_;
    int32_t raw_data_ = 0L;
};
#endif // SENSOR_H
