#include "Arduino.h"
#include "forcesensor.h"

ForceSensor::ForceSensor(LoadCell &cell) : cell_(cell) {}

ForceMeasurement_t ForceSensor::read(void)
{
    ForceMeasurement_t result;
    result.timestamp = millis();
    result.channel_a = cell_.read_channel_a();
    result.channel_b = cell_.read_channel_b();
    result.checksum = checksum(
        (const uint8_t*)&result,
        (sizeof(ForceMeasurement_t) - 1)
    );
    return result;
}

uint8_t ForceSensor::checksum(const uint8_t* data, uint16_t length)
{
    uint8_t checksum = 0U;
    for (uint16_t i = 0; i < length; i++)
    {
        checksum += data[i];
    }
    return checksum;
}