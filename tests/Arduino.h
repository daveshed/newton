#ifndef ARDUINO_H
#define ARDUINO_H
#include <cstdint>
#include <cstddef>
class SerialInterface;
class SerialHandle;

// functions and data available for code under test
extern SerialInterface Serial;
uint32_t millis(void);

// Wraps the mock serial handle/buffer to provide code under test code with the
// expected serial interface while allowing tests to access the handle directly
// and query its data.
class SerialInterface {
public:
    explicit SerialInterface(SerialHandle& h);
    void flush(void);
    size_t write(uint8_t* data, size_t n);
private:
    SerialHandle& handle_;
};

#endif // ARDUINO_H