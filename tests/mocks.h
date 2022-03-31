#ifndef MOCKS_H
#define MOCKS_H
#include "Arduino.h"

#include "forcesensor.h"

extern uint32_t millis_value_;

void set_fake_millis(uint32_t value);

class FakeLoadCell : public LoadCell {
public:
    int32_t read_channel_a(void) {return channel_a_;};
    int32_t read_channel_b(void) {return channel_b_;};
    void set_channel_a(int32_t value) {channel_a_ = value;};
    void set_channel_b(int32_t value) {channel_b_ = value;};

private:
    int32_t channel_a_ = 0L;
    int32_t channel_b_ = 0L;
};
#endif