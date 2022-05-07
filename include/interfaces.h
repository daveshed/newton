#ifndef INTERFACES_H
#define INTERFACES_H
#include "ArduinoUtils.h"

namespace Newton {

typedef uint8_t payload_size_t;
const size_t payload_limit = UCHAR_MAX;

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

enum struct Command {
    UNSPECIFIED = 0,
    CALIBRATE,
    GET_READING,
    READ_STORED,
    MAX,
};

enum struct Response {
    READ_OK = 0,
    CALIBRATE_OK,
    HAS_READINGS,
    NO_READINGS,
    BUSY,
    MAX,
};

typedef struct __attribute__((packed)) {
    uint32_t timestamp;
    float force;
    int32_t raw_data;
    uint8_t checksum;
} Measurement_t;

typedef struct __attribute__((packed)) {
    float slope;
    float intercept;
} Calibration_t;

struct ResponsePacket {
    struct Header {
        uint8_t type;
        payload_size_t payload_size;
    };
    Header header;
    uint8_t* payload;
};

struct CommandPacket {
    struct Header {
        uint8_t type;
        payload_size_t payload_size;
    };
    Header header;
    uint8_t* payload;
};

}; // namespace
#endif  // INTERFACES_H