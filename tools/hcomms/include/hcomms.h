#ifndef HOSTCOMMS_H
#define HOSTCOMMS_H
#include <vector>
#include "interfaces.h"

namespace Newton {

/*
 * The serial object used by both the host and target ends.
 */
class SerialHandle {
public:
    virtual void transmit(uint8_t to_transmit) = 0;
    virtual void transmit(const uint8_t* to_transmit, size_t n) = 0;
    virtual uint8_t receive(void) = 0;
    virtual void receive(uint8_t* result, size_t n) = 0;
    virtual size_t available(void) = 0;
};

class HostInterface {
public:
    explicit HostInterface(SerialHandle& serial);
    void calibrate(Calibration_t calibration) const;
    Measurement_t get_reading(void) const;
    std::vector<Measurement_t> read_stored(void) const;
private:
    SerialHandle& serial_;
};

} // namespace
#endif  // HOSTCOMMS_H