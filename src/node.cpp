#include "ArduinoUtils.h"
#include "node.h"

using namespace Newton;

Sensor::Sensor(void)
: calibration_{0, 0}
{}

void Sensor::calibrate(Calibration_t& calibration)
{
    calibration_ = calibration;
}

float Sensor::force(void) const
{
    return (raw_data_ * calibration_.slope) + calibration_.intercept;
}

Calibration_t Sensor::calibration(void) const
{
    return calibration_;
}

Node::Node(Sensor& sensor) : sensor_(sensor)
{}

void Node::begin(void)
{
    sensor_.begin();
}

Measurement_t Node::read(void)
{
    Measurement_t result;
    result.timestamp = millis();
    result.force = sensor_.force();
    result.raw_data = sensor_.raw_data();
    result.checksum = checksum_(
        (const uint8_t*)&result,
        (sizeof(Measurement_t) - 1)
    );
    return result;
}

void Node::update(void)
{
    sensor_.update();
}

void Node::transmit(void)
{
    Serial.flush();
    Measurement_t data = read();
    Serial.write((uint8_t*)&data, sizeof(Measurement_t));
}

uint8_t Node::checksum_(const uint8_t* data, uint16_t length)
{
    uint8_t checksum = 0U;
    for (uint16_t i = 0; i < length; i++)
    {
        checksum += data[i];
    }
    return checksum;
}
