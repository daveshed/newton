#ifndef TARGETCOMMS_H
#define TARGETCOMMS_H
#include "ArduinoUtils.h"
#include "interfaces.h"
#include "node.h"

namespace Newton {

class TargetDataReceived;
class TargetInterface;

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


class TargetInterface {
// handles serial data byte-by-byte
typedef void(*data_handler)(TargetInterface* iface);
public:
    friend TargetDataReceived;
    explicit TargetInterface(SerialHandle& serial, Sensor& sensor);
    void update(void);
    Sensor& sensor(void) const;
    SerialHandle& serial(void) const;

private:
    static void handle_calibrate_(TargetInterface* iface);
    static void handle_get_reading_(TargetInterface* iface);
    static void handle_request_(TargetInterface* iface);

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