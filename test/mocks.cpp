#include "mocks.h"

uint32_t millis_value_ = 0UL;

uint32_t millis(void)
{
    return millis_value_;
}

void set_fake_millis(uint32_t value)
{
    millis_value_ = value;
}