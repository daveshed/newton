#ifndef NODE_H
#define NODE_H
#include "ArduinoUtils.h"
#include "interfaces.h"

namespace Newton {

class TargetDataReceived;
class Node;

/*
 * Callback registered by the host to handle serial data from the target.
 */
class TargetDataReceived : public SerialDataCallback {
public:
    explicit TargetDataReceived(Node* iface);
    void operator()(void) override;

private:
    Node* iface_;
};

class Sensor
{
public:
    Sensor(void);
    virtual void begin(void) = 0;
    virtual int32_t raw_data(void) const = 0;
    virtual void update(void) = 0;
    float force(void) const;
    void calibrate(Calibration_t& calibration);
    Calibration_t calibration(void) const;

protected:
    int32_t raw_data_ = 0L;
    Calibration_t calibration_;
};

class Node {
// handles serial data byte-by-byte
typedef void(*data_handler)(Node* iface);
public:
    friend TargetDataReceived;
    explicit Node(SerialHandle& serial, Sensor& sensor);
    void update(void);
    Sensor& sensor(void) const;
    SerialHandle& serial(void) const;

private:
    // serial data handlers...
    static void handle_calibrate_(Node* iface);
    static void handle_get_reading_(Node* iface);
    static void handle_request_(Node* iface);

    uint8_t checksum_(const uint8_t* data, uint16_t length);

    SerialHandle& serial_;
    Sensor& sensor_;
    data_handler handler_ = handle_request_;
    // Even though this a pointer to this object is handed over to the serial
    // class immediately on construction, we need to retain ownership so it's
    // not deleted.
    TargetDataReceived serial_listener_;
};

} // namespace
#endif  // TARGETCOMMS_H