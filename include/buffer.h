#ifndef BUFFER_H
#define BUFFER_H
#include "ArduinoUtils.h"

namespace Newton {

template <typename T> class FifoBuffer {
public:
    static const uint8_t max_size = 64U;

    FifoBuffer(void);
    int pop(T* value);
    int pop(T* values, size_t n);
    int push(T value);
    int push(const T* values, size_t n);
    void clear(void);
    uint8_t size(void) const;
    uint8_t remaining(void) const;
    bool empty(void) const;
    bool full(void) const;
private:
    // Note: one element is wasted
    static const uint8_t length_ = max_size + 1U;
    T data_[length_];
    uint8_t head_;
    uint8_t tail_;
};

template <typename T> FifoBuffer<T>::FifoBuffer(void)
: head_(0U)
, tail_(0U)
{ }

template <typename T> int FifoBuffer<T>::pop(T* value)
{
    if (empty())
    {
        return -1;
    }
    if (value)
    {
        *value = data_[head_];
    }
    head_++;
    head_ %= length_;
    return 0;
}

template <typename T> int FifoBuffer<T>::pop(T* values, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        int result = pop(values + i);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

template <typename T> int FifoBuffer<T>::push(T value)
{
    if (full())
    {
        return -1;
    }
    data_[tail_] = value;
    tail_++;
    tail_ %= length_;
    return 0;
}

template <typename T> int FifoBuffer<T>::push(const T* values, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        int result = push(*(values + i));
        if (result)
        {
            return result;
        }
    }
    return 0;
}

template <typename T> void FifoBuffer<T>::clear(void)
{
    head_ = tail_ = 0U;
}

template <typename T> uint8_t FifoBuffer<T>::size(void) const
{
    return tail_ - head_;
}

template <typename T> uint8_t FifoBuffer<T>::remaining(void) const
{
    return max_size - size();
}

template <typename T> bool FifoBuffer<T>::empty(void) const
{
    return head_ == tail_;
}

template <typename T> bool FifoBuffer<T>::full(void) const
{
    return (tail_ + 1) % length_ == head_;
}


}; // namespace
#endif // BUFFER_H