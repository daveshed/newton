#ifndef INTERFACES_H
#define INTERFACES_H
#include "ArduinoUtils.h"
#include "limits.h"

namespace Newton {

typedef uint8_t payload_size_t;
const size_t payload_limit = UCHAR_MAX;

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