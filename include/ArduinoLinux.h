#ifndef ARDUINOLINUX_H
#define ARDUINOLINUX_H
#include <cstdint>
#include <cstddef>
uint32_t millis(void);
#if 0
class SerialInterface;
class SerialHandle;

// functions and data available for code under test
extern SerialInterface Serial;

// Wraps the mock serial handle/buffer to provide code under test code with the
// expected serial interface while allowing tests to access the handle directly
// and query its data.
class SerialInterface {
public:
    explicit SerialInterface(SerialHandle& h);
    void flush(void);
    int read(void);
    size_t write(uint8_t* data, size_t n);
    int available(void);

private:
    SerialHandle& handle_;
};
#endif
#endif // ARDUINOLINUX_H