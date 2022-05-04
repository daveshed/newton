// implement serial interface required by node
#include "ArduinoUtils.h"
#include "buffer.h"
#include "logging.h"
#include "serial.h"

using namespace Newton;

ArduinoSerialHandle::ArduinoSerialHandle(
    FifoBuffer& tx, FifoBuffer& rx)
: callback_(nullptr)
, tx_queue_(tx)
, rx_queue_(rx)
{ }

void ArduinoSerialHandle::transmit(uint8_t to_transmit)
{
    LOG_DEBUG("Queuing byte to transmit...");
    LOG_DEBUG(to_transmit);
    tx_queue_.push(to_transmit);
}

void ArduinoSerialHandle::transmit(const uint8_t* to_transmit, size_t n)
{
    LOG_DEBUG("Queuing bytes to transmit...");
    for (size_t i = 0; i < n; ++i)
    {
        transmit(to_transmit[i]);
    }
}

uint8_t ArduinoSerialHandle::receive(void)
{
    LOG_DEBUG("Retrieving a byte...");
    uint8_t result;
    rx_queue_.pop(&result);
    return result;
}

void ArduinoSerialHandle::receive(uint8_t* result, size_t n)
{
    LOG_DEBUG("Retrieving bytes...");
    for (size_t i = 0; i < n; ++i)
    {
        result[i] = receive();
    }
}

size_t ArduinoSerialHandle::available(void)
{
    return rx_queue_.size();
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
