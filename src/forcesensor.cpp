#include "Arduino.h"
#include "forcesensor.h"

using namespace ForceSensor;

Plate::Plate(Sensor& x, Sensor& y, Sensor& z) :
sensor_x_(x),
sensor_y_(y),
sensor_z_(z) {}

Measurement_t Plate::read(void)
{
    Measurement_t result;
    result.timestamp = millis();
    result.force_x = sensor_x_.newtons();
    result.force_y = sensor_y_.newtons();
    result.force_z = sensor_z_.newtons();
    result.data_x = sensor_x_.raw_data();
    result.data_y = sensor_y_.raw_data();
    result.data_z = sensor_z_.raw_data();
    result.checksum = checksum_(
        (const uint8_t*)&result,
        (sizeof(Measurement_t) - 1)
    );
    return result;
}

void Plate::update(void)
{
    sensor_x_.update();
    sensor_y_.update();
    sensor_z_.update();
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
