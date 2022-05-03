#include "ArduinoUtils.h"
#include "interfaces.h"
#include "logging.h"
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

TargetDataReceived::TargetDataReceived(Node* iface) : iface_(iface)
{}

void TargetDataReceived::operator()(void)
{
    LOG_DEBUG("handling serial data...");
    iface_->handler_(iface_);
}

Node::Node(
    SerialHandle& serial,
    Sensor& sensor
)
: serial_(serial)
, sensor_(sensor)
, serial_listener_(TargetDataReceived(this))
{
    serial_.register_callback(&serial_listener_);
}

Sensor& Node::sensor(void) const
{
    return sensor_;
}

Newton::SerialHandle& Node::serial(void) const
{
    return serial_;
}

void Node::update(void)
{
    sensor_.update();
}

void Node::handle_calibrate_(Node* iface)
{
    LOG_DEBUG("handling calibration request...");
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

void Node::handle_get_reading_(Node* iface)
{
    LOG_DEBUG("handling get reading request...");
    Measurement_t result = {
        millis(),
        iface->sensor().force(),
        iface->sensor().raw_data(),
        0, // FIXME: calculate checksum
    };
    iface->serial().transmit((const uint8_t*)&result, sizeof(Measurement_t));
    LOG_DEBUG("done.");
    // done
    iface->handler_ = handle_request_;
}

void Node::handle_request_(Node* iface)
{
    // lookup table...
    data_handler handlers[COMMAND_MAX] = {
        handle_calibrate_,
        handle_get_reading_,
    };
    // Not handling anything. Read the next command from serial...
    uint8_t command_type = iface->serial().receive();
    // execute it on this...
    LOG_DEBUG("Got command:");
    LOG_DEBUG(static_cast<int>(command_type));
    // Find the correct handler and call it to dispatch the data.
    iface->handler_ = handlers[command_type];
    iface->handler_(iface);
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
