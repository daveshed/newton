// third party
#include "Arduino.h"
// project imports
#include "node.h"
#include "logging.h"
#include "serialcomms.h"

// Private helper reads from serial into a buffer. A template function might
// be a good idea here. It could read from Serial and create the type required.
// eg auto foo = read_serial<FooT>(); ?
// For now let's do this...
void read_serial(uint8_t* data, size_t n)
{
    LOG_DEBUG("read_serial:");
    for (size_t i = 0; i < n; i++)
    {
        data[i] = Serial.read();
        LOG_DEBUG(" ");
        LOG_DEBUG(data[i]);
    }
    LOG_DEBUG("\n");
}

Newton::Calibration_t read_calibration()
{
    Newton::Calibration_t result;
    read_serial((uint8_t*)&result, sizeof(Newton::Calibration_t));
    return result;
}

CommandInterface::CommandInterface(Newton::Sensor& sensor) :
sensor_(sensor)
{}

void CommandInterface::update(void)
{
    if(Serial.available())
    {
        Newton::Calibration_t calib = read_calibration();
        sensor_.calibrate(calib);
    }
}
