#include "hostcomms.h"
#include "interfaces.h"
#include "logging.h"

using namespace Newton;

HostInterface::HostInterface(Newton::SerialHandle& s) : serial_(s)
{ }

void HostInterface::calibrate(Calibration_t calibration) const
{
    LOG_DEBUG("##### Host issuing calibration...");
    // command type
    serial_.transmit(CALIBRATE_COMMAND);
    // payload
    serial_.transmit((const uint8_t*)&calibration, sizeof(Calibration_t));
}

Measurement_t HostInterface::get_reading(void)
{
    LOG_DEBUG("##### Host getting reading...");
    // 1. issue the command...
    serial_.transmit(GET_READING_COMMAND);
    // 2. wait for the response - this needs consideration. What if the data
    //    from the target takes a long time to come? Will we just let timeout
    //    exceptions raise?
    Measurement_t result = {};
    serial_.receive((uint8_t*)&result, sizeof(Measurement_t));
    return result;
}
