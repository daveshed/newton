#ifndef INTERFACES_H
#define INTERFACES_H
#include "ArduinoUtils.h"

namespace Newton {

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
 * The serial object used by both the host and target ends.
 */
class SerialHandle {
public:
    virtual void transmit(uint8_t to_transmit) = 0;
    virtual void transmit(const uint8_t* to_transmit, size_t n) = 0;
    virtual uint8_t receive(void) = 0;
    virtual void receive(uint8_t* result, size_t n) = 0;
    virtual size_t available(void) = 0;
    virtual void register_callback(SerialDataCallback* callback) = 0;
};

enum commands {
    CALIBRATE_COMMAND,
    GET_READING_COMMAND,
    COMMAND_MAX,
};

typedef struct __attribute__((packed)) Measurement_s {
    uint32_t timestamp;
    float force;
    int32_t raw_data;
    uint8_t checksum;
} Measurement_t;

typedef struct {
    float slope;
    float intercept;
} Calibration_t;

}; // namespace

#endif  // INTERFACES_H