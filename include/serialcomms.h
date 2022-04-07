#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H
#include "ArduinoUtils.h"
#include "node.h"

namespace Newton {

class HostInterface;
class Serial;
class SerialDataCallback;
class TargetDataReceived;
class TargetInterface;

/*
 * Callback that can be registered with the Serial class. Whenever data arrives
 * (a single byte), this callback will be called.
 *
 * Note that it's an object so that the callback can have access to instance
 * data that isn't available in a lambda for example (we can't use boost or
 * bind here).
 */
class SerialDataCallback {
public:
    virtual void operator()(void) = 0;
};

/*
 * Callback registered by the host to handle serial data from the target.
 */
class TargetDataReceived : public SerialDataCallback {
public:
    explicit TargetDataReceived(TargetInterface* iface);
    void operator()(void) override;

private:
    TargetInterface* iface_;
};

/*
 * The serial object used by both the host and target ends.
 */
class Serial {
public:
    virtual void transmit(uint8_t to_transmit) = 0;
    virtual void transmit(const uint8_t* to_transmit, size_t n) = 0;
    virtual uint8_t receive(void) = 0;
    virtual void receive(uint8_t* result, size_t n) = 0;
    virtual size_t available(void) = 0;
    virtual void register_callback(SerialDataCallback* callback) = 0;
};

class HostInterface {
public:
    explicit HostInterface(Serial& serial);
    void calibrate(Calibration_t calibration) const;
    Measurement_t get_reading(void);
private:
    Serial& serial_;
};

class TargetInterface {
// handles serial data byte-by-byte
typedef void(*data_handler)(TargetInterface* iface);
public:
    friend TargetDataReceived;
    explicit TargetInterface(Serial& serial, Sensor& sensor);
    void update(void);
    Sensor& sensor(void) const;
    Serial& serial(void) const;

private:
    static void handle_calibrate_(TargetInterface* iface);
    static void handle_get_reading_(TargetInterface* iface);
    static void handle_request_(TargetInterface* iface);

    Serial& serial_;
    Sensor& sensor_;
    data_handler handler_ = handle_request_;
    // Even though this a pointer to this object is handed over to the serial
    // class immediately on construction, we need to retain ownership so it's
    // not deleted.
    TargetDataReceived serial_listener_;
};

} // namespace
#endif