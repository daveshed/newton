#include <cstddef>
#include <cstring>

#include "Arduino.h"
#include "mocks.h"

// exported public data
SerialHandle serial_handle;
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
    handle_.flush();
}

size_t SerialInterface::write(uint8_t* data, size_t n)
{
    handle_.write(data, n);
    return n;
}
