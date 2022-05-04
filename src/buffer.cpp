#include "ArduinoUtils.h"
#include "buffer.h"

using namespace Newton;

FifoBuffer::FifoBuffer(void)
: head_(0U)
, tail_(0U)
{ }

int FifoBuffer::pop(uint8_t* result)
{
    if (empty())
    {
        return -1;
    }
    if (result)
    {
        *result = data_[head_];
    }
    head_++;
    head_ %= FIFO_SIZE;
    return 0;
}

int FifoBuffer::push(uint8_t value)
{
    if (full())
    {
        return -1;
    }
    data_[tail_] = value;
    tail_++;
    tail_ %= FIFO_SIZE;
    return 0;
}

void FifoBuffer::clear(void)
{
    head_ = tail_ = 0U;
}

uint8_t FifoBuffer::size(void) const
{
    return tail_ - head_;
}

bool FifoBuffer::empty(void) const
{
    return head_ == tail_;
}

bool FifoBuffer::full(void) const
{
    return (tail_ + 1) % FIFO_SIZE == head_;
}
