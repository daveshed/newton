#include "ArduinoUtils.h"
#include "node.h"
#include "logging.h"
#include "serialcomms.h"

#include <string>
#include <map>

using namespace Newton;

typedef void(*command_handler)(TargetInterface* iface);


void handle_calibrate(TargetInterface* iface)
{
    LOG_DEBUG("Calibrating...\n");
    // read the calibration data from serial...
    Calibration_t calibration;
    iface->serial().receive((uint8_t*)&calibration, sizeof(Calibration_t));
    // apply it to the sensor...
    iface->sensor().calibrate(calibration);
}

std::map<uint8_t, command_handler> handlers = {
    {0x00, handle_calibrate},
};

HostInterface::HostInterface(Newton::Serial& s) : serial_(s) {}

void HostInterface::calibrate(Calibration_t calibration) const
{
    // command type
    serial_.transmit(0x00);
    // payload
    serial_.transmit((const uint8_t*)&calibration, sizeof(Calibration_t));
}

TargetInterface::TargetInterface(
    Serial& serial,
    Sensor& sensor
)
: serial_(serial)
, sensor_(sensor)
{ }

Sensor& TargetInterface::sensor(void) const
{
    return sensor_;
}

Newton::Serial& TargetInterface::serial(void) const
{
    return serial_;
}

void TargetInterface::update(void)
{
    // read the command from serial...
    uint8_t command_type = serial_.receive();
    // execute it on this...
    LOG_DEBUG(command_type);
    handlers[command_type](this);
}

CalibrateCommand::CalibrateCommand(Calibration_t calib)
: calibration_(calib)
{}

void CalibrateCommand::execute(TargetInterface& iface)
{
    iface.sensor().calibrate(calibration_);
}




// Private helper reads from serial into a buffer. A template function might
// be a good idea here. It could read from Serial and create the type required.
// eg auto foo = read_serial<FooT>(); ?
// For now let's do this...
// void read_serial(uint8_t* data, size_t n)
// {
//     LOG_DEBUG("read_serial:");
//     for (size_t i = 0; i < n; i++)
//     {
//         data[i] = Serial.read();
//         LOG_DEBUG(" ");
//         LOG_DEBUG(data[i]);
//     }
//     LOG_DEBUG("\n");
// }

// Calibration_t read_calibration()
// {
//     Newton::Calibration_t result;
//     read_serial((uint8_t*)&result, sizeof(Newton::Calibration_t));
//     return result;
// }

// CommandInterface::CommandInterface(Newton::Sensor& sensor) :
// sensor_(sensor)
// {}

// void Newton::CommandInterface::update(void)
// {
//     if(Serial.available())
//     {
//         Newton::Calibration_t calib = read_calibration();
//         sensor_.calibrate(calib);
//     }
// }
