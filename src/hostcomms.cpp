// stdlib
#include <cstring>
#include <vector>
// internal
#include "hostcomms.h"
#include "interfaces.h"
#include "logging.h"

using namespace Newton;

HostInterface::HostInterface(SerialHandle& s) : serial_(s)
{ }

void HostInterface::calibrate(Calibration_t calibration) const
{
    LOG_DEBUG("##### Host issuing calibration...");
    // command type
    serial_.transmit((uint8_t)Command::CALIBRATE);
    // payload
    serial_.transmit((const uint8_t*)&calibration, sizeof(Calibration_t));
}

Measurement_t HostInterface::get_reading(void) const
{
    LOG_DEBUG("##### Host getting reading...");
    // 1. issue the command...
    serial_.transmit((uint8_t)Command::GET_READING);
    // 2. wait for the response - this needs consideration. What if the data
    //    from the target takes a long time to come? Will we just let timeout
    //    exceptions raise?
    Measurement_t result = {};
    serial_.receive((uint8_t*)&result, sizeof(Measurement_t));
    return result;
}

std::vector<Measurement_t> HostInterface::read_stored(void) const
{
    LOG_DEBUG("##### Host reading stored...");
    serial_.transmit((uint8_t)Command::READ_STORED);
    uint8_t count = serial_.receive();
    std::vector<Measurement_t> result;
    for (int i = 0; i < count; ++i)
    {
        Measurement_t reading = {};
        serial_.receive((uint8_t*)&result, sizeof(Measurement_t));
        result.push_back(reading);
    }
    return result;
}
