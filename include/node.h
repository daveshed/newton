#ifndef NODE_H
#define NODE_H
#include "ArduinoUtils.h"
#include "buffer.h"
#include "interfaces.h"

namespace Newton {

class DataRecorder;
class Node;
class NodeCodec;
class Sensor;
Sensor* make_force_sensor(void);

class Sensor
{
public:
    Sensor(void);
    virtual void begin(void) = 0;
    virtual int32_t raw_data(void) const = 0;
    virtual void raw_data(int32_t value) = 0;
    virtual void update(void) = 0;
    virtual float force(void) const;
    void calibrate(Calibration_t& calibration);
    Calibration_t calibration(void) const;

protected:
    int32_t raw_data_ = 0L;
    Calibration_t calibration_;
};

// Responsible for handling data over the serial connection - declaration should live in a private
// header.
class NodeCodec {
public:
    typedef void (NodeCodec::*handler_type)(void);
    NodeCodec(Node* node);
    void update(void);
private:
    // serial data handlers
    void handle_calibrate_(void);
    void handle_get_reading_(void);
    void handle_request_(void);
    void handle_read_stored_(void);
    void handle_transmit_stored_payload_(void);
    // helpers
    void send_response_(const ResponsePacket::Header& header) const;
    void send_response_(Response type, payload_size_t size, const uint8_t* payload) const;
    // private data
    Node* node_;
    CommandPacket::Header header_;
    handler_type handler_;
};

class Node {
public:
    constexpr static float threshold = 10.0F;
    constexpr static uint8_t samples = 100U;

    explicit Node(FifoBuffer<uint8_t>& tx, FifoBuffer<uint8_t>& rx, Sensor& sensor);
    void update(void);
    Sensor& sensor(void) const;
    FifoBuffer<uint8_t>& tx(void) const;
    FifoBuffer<uint8_t>& rx(void) const;
    FifoBuffer<Measurement_t>& buffer(void);
    Measurement_t reading(void) const;
    bool is_measuring(void) const;

private:
    // helpers
    uint8_t checksum_(const uint8_t* data, uint16_t length);

    FifoBuffer<uint8_t>& tx_;
    FifoBuffer<uint8_t>& rx_;
    FifoBuffer<Measurement_t> buffer_;
    Sensor& sensor_;
    NodeCodec codec_;
    Measurement_t reading_;
};

} // namespace
#endif  // NODE_H