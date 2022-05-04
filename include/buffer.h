#ifndef BUFFER_H
#define BUFFER_H
#include "ArduinoUtils.h"

#define FIFO_SIZE    (64U)

namespace Newton {

class FifoBuffer {
public:
    FifoBuffer(void);
    int pop(uint8_t* value);
    int push(uint8_t value);
    void clear(void);
    uint8_t size(void) const;
    bool empty(void) const;
    bool full(void) const;
private:
    uint8_t data_[FIFO_SIZE];
    uint8_t head_;
    uint8_t tail_;
};

}; // namespace
#endif // BUFFER_H