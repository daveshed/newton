// mock api only available to tests
#ifndef MOCKS_H
#define MOCKS_H
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

#include "Arduino.h"

#include "forcesensor.h"

// make the underlying serial handle available for tests to read buffer.
extern SerialHandle serial_handle;

// allow tests to modify timestamp
void millis(uint32_t value);

// Holds data in the underlying serial buffers exposed by Arduino.h Serial
struct SerialHandle {
    static const int buffer_size = 64;   // from arduino.cc
    bool flushed = false;
    // mock methods for tests only...
    std::vector<uint8_t> rx;  // host->target(arduino)
    std::vector<uint8_t> tx;  // target->host
    void reset(void) {
        rx.clear();
        tx.clear();
        flushed = false;
    };
    void print() {
        std::cout << "rx:";
        for (auto i = rx.begin(); i != rx.end(); ++i)
        {
            std::cout << " " << *i;
        }
        std::cout << std::endl;
    };
};

class FakeSensor : public ForceSensor::Sensor {
public:
    int32_t raw_data(void) const override {
        assert(updated);
        return raw_data_;
    };
    void update(void) override {
        raw_data_ = new_data_;
        newtons_ = new_newtons_;
        updated = true;
    };
    // mock methods available to tests...
    void raw_data(int32_t value) {
        new_data_ = value;
        updated = false;
    };

private:
    int32_t new_data_ = 0L;
    float newtons_ = 0.0;
    float new_newtons_ = 0.0;
    bool updated = false;
};
#endif