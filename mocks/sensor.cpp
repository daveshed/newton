// stub implementation
#include "ArduinoUtils.h"
#include "sensor_stub.h"
#include "node.h"

FakeForceSensor::FakeForceSensor() {};

void FakeForceSensor::begin(void) {};

int32_t FakeForceSensor::raw_data(void) const
{
    ASSERT(updated);
        return raw_data_;
}

void FakeForceSensor::update(void)
{
    raw_data_ = new_data_;
    updated = true;
}

void FakeForceSensor::raw_data(int32_t value)
{
    new_data_ = value;
    updated = false;
};
