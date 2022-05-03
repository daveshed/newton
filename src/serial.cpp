// implement serial interface required by node
#include <Wire.h>

#include "ArduinoUtils.h"
#include "logging.h"
#include "serial.h"

using namespace Newton;

ArduinoSerialHandle::ArduinoSerialHandle(uint8_t address) : callback_(nullptr)
{
    LOG_DEBUG("Opening i2c connection...");
    Wire.begin(address);
}

ArduinoSerialHandle::~ArduinoSerialHandle(void)
{
    LOG_DEBUG("Closing i2c connection...");
    Wire.end();
}

void ArduinoSerialHandle::transmit(uint8_t to_transmit)
{
    LOG_DEBUG("Transmitting byte...");
    LOG_DEBUG(to_transmit);
    Wire.write(to_transmit);
}

void ArduinoSerialHandle::transmit(const uint8_t* to_transmit, size_t n)
{
    LOG_DEBUG("Transmitting bytes...");
    Wire.write(to_transmit, n);
}

uint8_t ArduinoSerialHandle::receive(void)
{
    return Wire.read();
}

void ArduinoSerialHandle::receive(uint8_t* result, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        result[i] = receive();
    }
}

size_t ArduinoSerialHandle::available(void)
{
    return Wire.available();
}

void ArduinoSerialHandle::register_callback(SerialDataCallback* callback)
{
    callback_ = callback;
}

void ArduinoSerialHandle::notify(void) const
{
    if (callback_)
    {
        (*callback_)();
    }
}
