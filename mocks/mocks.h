// mock api only available to tests
#ifndef MOCKS_H
#define MOCKS_H
#include <vector>

#include "ArduinoLinux.h"
#include "logging.h"

// allow tests to modify timestamp
void millis(uint32_t value);

#if 0
// make the underlying serial handle available for tests to read buffer.
extern SerialHandle serial_handle;


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
        LOG_DEBUG("rx:");
        for (auto i = rx.begin(); i != rx.end(); ++i)
        {
            LOG_DEBUG(" ");
            LOG_DEBUG(*i);
        }
        std::cout << std::endl;
    };
};
#endif
#endif // MOCKS_H
