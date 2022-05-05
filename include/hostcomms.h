#ifndef HOSTCOMMS_H
#define HOSTCOMMS_H
#include "interfaces.h"

namespace Newton {

class HostInterface {
public:
    explicit HostInterface(SerialHandle& serial);
    void calibrate(Calibration_t calibration) const;
    Measurement_t get_reading(void) const;
private:
    SerialHandle& serial_;
};

} // namespace
#endif  // HOSTCOMMS_H