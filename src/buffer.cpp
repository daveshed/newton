#include "ArduinoUtils.h"
#include "buffer.h"

using namespace Newton;

FifoBuffer::FifoBuffer(void)
{
    clear();
}

uint8_t FifoBuffer::pop(void)
{
    uint8_t result = *head_;
    head_++;
    return result;
}

void FifoBuffer::push(uint8_t val)
{
    *tail_ = val;
    tail_++;
}

void FifoBuffer::clear(void)
{
    head_ = tail_ = data_;
}

uint8_t FifoBuffer::size(void) const
{
    return tail_ - head_;
}

bool FifoBuffer::empty(void) const
{
    return head_ == tail_;
}
