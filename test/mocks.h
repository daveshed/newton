// mock api only available to tests
#ifndef MOCKS_H
#define MOCKS_H
#include <cassert>
#include <cstring>
#include "Arduino.h"
#include "../forcesensor.h"

// make the underlying serial handle available for tests to read buffer.
extern SerialHandle serial_handle;

// allow tests to modify timestamp
void millis(uint32_t value);

// allow tests to inspect/reset the serial buffer
class SerialHandle {
public:
    SerialHandle() {reset();};
    void flush(void) {flushed_ = true;};
    size_t write(uint8_t* data, size_t n) {
        assert(flushed_);  // ensure we block for buffer to flush prior to write
        memcpy(buffer_, data, n);
        flushed_ = false;
        return n;
    };
    // mock methods for tests only...
    uint8_t* buffer(void) {return buffer_;};
    void reset(void) {
        memset(buffer_, 0U, buffer_size_);
        flushed_ = false;
    };
private:
    static const int buffer_size_ = 255;
    uint8_t buffer_[buffer_size_];
    bool flushed_;
};

class FakeSensor : public ForceSensor::Sensor {
public:
    float newtons(void) {
        assert(updated);
        return newtons_;
    };
    int32_t raw_data(void) {
        assert(updated);
        return raw_data_;
    };
    void update(void) {
        raw_data_ = new_data_;
        newtons_ = new_newtons_;
        updated = true;
    };
    // mock methods available to tests...
    void newtons(float value) {
        new_newtons_ = value;
        updated = false;
    };
    void raw_data(int32_t value) {
        new_data_ = value;
        updated = false;
    };

private:
    int32_t raw_data_ = 0L;
    int32_t new_data_ = 0L;
    float newtons_ = 0.0;
    float new_newtons_ = 0.0;
    bool updated = false;
};
#endif