#include "Arduino.h"
#include "forcesensor.h"

using namespace ForceSensor;

Plate::Plate(Sensor& sensor) : sensor_(sensor)
{}

Measurement_t Plate::read(void)
{
    Measurement_t result;
    result.timestamp = millis();
    result.newtons = sensor_.newtons();
    result.raw_data = sensor_.raw_data();
    result.checksum = checksum_(
        (const uint8_t*)&result,
        (sizeof(Measurement_t) - 1)
    );
    return result;
}

void Plate::update(void)
{
    sensor_.update();
}

void Plate::transmit(void)
{
    Serial.flush();
    Measurement_t data = read();
    Serial.write((uint8_t*)&data, sizeof(Measurement_t));
}

uint8_t Plate::checksum_(const uint8_t* data, uint16_t length)
{
    uint8_t checksum = 0U;
    for (uint16_t i = 0; i < length; i++)
    {
        checksum += data[i];
    }
    return checksum;
}
