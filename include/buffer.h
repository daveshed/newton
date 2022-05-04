#ifndef BUFFER_H
#define BUFFER_H
#include "ArduinoUtils.h"

namespace Newton {

class FifoBuffer {
public:
    static const uint8_t max_size = 64U;

    FifoBuffer(void);
    int pop(uint8_t* value);
    int push(uint8_t value);
    void clear(void);
    uint8_t size(void) const;
    bool empty(void) const;
    bool full(void) const;
private:
    // Note: one element is wasted
    static const uint8_t length_ = max_size + 1U;
    uint8_t data_[length_];
    uint8_t head_;
    uint8_t tail_;
};

}; // namespace
#endif // BUFFER_H