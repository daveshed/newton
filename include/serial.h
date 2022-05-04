// implements the serial interface expected by Newton::Node on the target1
#ifndef SERIAL_H
#define SERIAL_H
#include "ArduinoUtils.h"
#include "buffer.h"
#include "interfaces.h"
#include "node.h"

namespace Newton {

class ArduinoSerialHandle : public SerialHandle {
public:
    ArduinoSerialHandle(FifoBuffer& tx, FifoBuffer& rx);
    void transmit(uint8_t to_transmit) override;
    void transmit(const uint8_t* to_transmit, size_t n) override;
    uint8_t receive(void) override;
    void receive(uint8_t* result, size_t n) override;
    size_t available(void) override;
    void register_callback(SerialDataCallback* callback) override;
    void notify(void) const;

private:
    SerialDataCallback* callback_;
    FifoBuffer& tx_queue_;
    FifoBuffer& rx_queue_;
};

} // namespace
#endif // SERIAL_H