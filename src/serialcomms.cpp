#include "ArduinoUtils.h"
#include "node.h"
#include "logging.h"
#include "serialcomms.h"

using namespace Newton;

enum commands {
    CALIBRATE_COMMAND,
    GET_READING_COMMAND,
    COMMAND_MAX,
};

HostInterface::HostInterface(Newton::Serial& s) : serial_(s)
{ }

void HostInterface::calibrate(Calibration_t calibration) const
{
    LOG_DEBUG("##### Host issuing calibration...\n");
    // command type
    serial_.transmit(CALIBRATE_COMMAND);
    // payload
    serial_.transmit((const uint8_t*)&calibration, sizeof(Calibration_t));
}

Measurement_t HostInterface::get_reading(void)
{
    LOG_DEBUG("##### Host getting reading...\n");
    // 1. issue the command...
    serial_.transmit(GET_READING_COMMAND);
    // 2. wait for the response - this needs consideration. What if the data
    //    from the target takes a long time to come? Will we just let timeout
    //    exceptions raise?
    Measurement_t result = {};
    serial_.receive((uint8_t*)&result, sizeof(Measurement_t));
    return result;
}

TargetDataReceived::TargetDataReceived(TargetInterface* iface) : iface_(iface)
{}

void TargetDataReceived::operator()(void)
{
    LOG_DEBUG("handling serial data...\n");
    iface_->handler_(iface_);
}

TargetInterface::TargetInterface(
    Serial& serial,
    Sensor& sensor
)
: serial_(serial)
, sensor_(sensor)
, serial_listener_(TargetDataReceived(this))
{
    serial_.register_callback(&serial_listener_);
}

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
    sensor_.update();
}

void TargetInterface::handle_calibrate_(TargetInterface* iface)
{
    LOG_DEBUG("handling calibration request...\n");
    // read the calibration data from serial when it's arrived...
    if (iface->serial().available() < sizeof(Calibration_t))
    {
        return;
    }
    Calibration_t calibration;
    iface->serial().receive((uint8_t*)&calibration, sizeof(Calibration_t));
    // apply it to the sensor...
    iface->sensor().calibrate(calibration);
    // done
    iface->handler_ = handle_request_;
}

void TargetInterface::handle_get_reading_(TargetInterface* iface)
{
    LOG_DEBUG("handling get reading request...\n");
    Measurement_t result = {
        millis(),
        iface->sensor().force(),
        iface->sensor().raw_data(),
        0, // FIXME: calculate checksum
    };
    iface->serial().transmit((const uint8_t*)&result, sizeof(Measurement_t));
    // done
    iface->handler_ = handle_request_;
}

void TargetInterface::handle_request_(TargetInterface* iface)
{
    // lookup table...
    data_handler handlers[COMMAND_MAX] = {
        handle_calibrate_,
        handle_get_reading_,
    };
    // Not handling anything. Read the next command from serial...
    uint8_t command_type = iface->serial().receive();
    // execute it on this...
    LOG_DEBUG("Got command <");
    LOG_DEBUG(static_cast<int>(command_type));
    LOG_DEBUG(">\n");
    // Find the correct handler and call it to dispatch the data.
    iface->handler_ = handlers[command_type];
    iface->handler_(iface);
}
