#ifndef __AVR__
#include "ArduinoUtils.h"
#include "mocks.h"

// export internal serial handle for tests to access data
SerialHandle serial_handle;
// export Serial object that mocks Arduino.h Serial
SerialInterface Serial = SerialInterface(serial_handle);
// private data
static uint32_t g_millis_value = 0L;

uint32_t millis(void)
{
    return g_millis_value;
}

void millis(uint32_t value)
{
    g_millis_value = value;
}

SerialInterface::SerialInterface(SerialHandle& h): handle_(h)
{}

void SerialInterface::flush(void)
{
    handle_.flushed = true;
}

size_t SerialInterface::write(uint8_t* data, size_t n)
{
    assert(handle_.flushed);  // ensure we block for buffer to flush
    assert((handle_.tx.size() + n) < handle_.buffer_size);
    for (size_t i = 0; i < n; ++i)
    {
        handle_.tx.push_back(data[i]);
    }
    handle_.flushed = false;
    return n;
}

// TODO - should return -1 if no data is available
int SerialInterface::read(void)
{
    // A deque would make more sense here
    auto& buffer = handle_.rx;
    uint8_t result = buffer.front();
    buffer.erase(buffer.begin());
    return result;
}

int SerialInterface::available(void)
{
    return handle_.rx.size();
}
#endif