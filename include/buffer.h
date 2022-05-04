#ifndef BUFFER_H
#define BUFFER_H
#include "ArduinoUtils.h"

#define FIFO_SIZE    64

namespace Newton {

class FifoBuffer {
public:
    FifoBuffer(void);
    uint8_t pop(void);
    void push(uint8_t val);
    void clear(void);
    uint8_t size(void) const;
    bool empty(void) const;
private:
    uint8_t data_[FIFO_SIZE];
    uint8_t* head_;
    uint8_t* tail_;
};

}; // namespace
#endif // BUFFER_H