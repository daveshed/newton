// Concrete serial comms interface for hcomms
#pragma once
#include <cstdint>
#include "interfaces.h"

namespace HostComms {

class I2cSerial : public Newton::SerialHandle {
public:
    I2cSerial(unsigned i2c_bus, unsigned i2c_address);
    ~I2cSerial(void);
    void transmit(uint8_t to_transmit) override;
    void transmit(const uint8_t* to_transmit, size_t n) override;
    uint8_t receive(void) override;
    void receive(uint8_t* result, size_t n) override;
    size_t available(void) override;
    void register_callback(Newton::SerialDataCallback* callback) override;

private:
    unsigned handle_;
};

} // namespace