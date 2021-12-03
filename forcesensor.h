#ifndef FORCESENSOR_H
#define FORCESENSOR_H

#include "Arduino.h"

typedef struct __attribute__((packed)) ForceMeasurement_s
{
    uint32_t timestamp;
    int32_t channel_a;
    int32_t channel_b;
    uint8_t checksum;
} ForceMeasurement_t;

class LoadCell
{
public:
    virtual int32_t read_channel_a(void) = 0;
    virtual int32_t read_channel_b(void) = 0;
};

class ForceSensor
{
public:
    explicit ForceSensor(LoadCell&);
    ForceMeasurement_t read(void);

private:
    uint8_t checksum(const uint8_t* data, uint16_t length);
    LoadCell& cell_;
};

#endif