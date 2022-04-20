#include "ArduinoUtils.h"
#include "serial.h"

using namespace Newton;

ArduinoSerialHandle::ArduinoSerialHandle(void) : callback_(nullptr)
{}

void ArduinoSerialHandle::transmit(uint8_t to_transmit)
{
    Serial.write(to_transmit);
}

void ArduinoSerialHandle::transmit(const uint8_t* to_transmit, size_t n)
{
    Serial.write(to_transmit, n);
}

uint8_t ArduinoSerialHandle::receive(void)
{
    return Serial.read();
}

void ArduinoSerialHandle::receive(uint8_t* result, size_t n)
{
    Serial.readBytes(result, n);
}

size_t ArduinoSerialHandle::available(void)
{
    return Serial.available();
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
