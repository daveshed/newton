#include <iostream>

#include "Arduino.h"
#include "forcesensor.h"
#include "serialcomms.h"

// Private helper reads from serial into a buffer. A template function might
// be a good idea here. It could read from Serial and create the type required.
// eg auto foo = read_serial<FooT>(); ?
// For now let's do this...
void read_serial(uint8_t* data, size_t n)
{
    std::cout << "read_serial:";
    for (int i = 0; i < n; i++)
    {
        data[i] = Serial.read();
        std::cout << " " << data[i];
    }
    std::cout << std::endl;
}

ForceSensor::Calibration_t read_calibration()
{
    ForceSensor::Calibration_t result;
    read_serial((uint8_t*)&result, sizeof(ForceSensor::Calibration_t));
    return result;
}

CommandInterface::CommandInterface(ForceSensor::Sensor& sensor) :
sensor_(sensor)
{}

void CommandInterface::update(void)
{
    if(Serial.available())
    {
        ForceSensor::Calibration_t calib = read_calibration();
        sensor_.calibrate(calib);
    }
}
