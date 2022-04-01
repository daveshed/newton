// mock api only available to tests
#ifndef MOCKS_H
#define MOCKS_H
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

#include "Arduino.h"

#include "node.h"

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
        // clean up allocated memory...
        rx.shrink_to_fit();
        tx.shrink_to_fit();
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

class FakeSensor : public Newton::Sensor {
public:
    int32_t raw_data(void) const override {
        assert(updated);
        return raw_data_;
    };
    void update(void) override {
        raw_data_ = new_data_;
        force_ = new_force_;
        updated = true;
    };
    // mock methods available to tests...
    void raw_data(int32_t value) {
        new_data_ = value;
        updated = false;
    };

private:
    int32_t new_data_ = 0L;
    float force_ = 0.0;
    float new_force_ = 0.0;
    bool updated = false;
};
#endif