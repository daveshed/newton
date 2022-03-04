#ifndef FORCESENSOR_H
#define FORCESENSOR_H

#include "Arduino.h"  // cstdint

namespace ForceSensor {
    class Sensor;
    class Plate;

    typedef struct __attribute__((packed)) Measurement_s {
        uint32_t timestamp;
        float newtons;
        int32_t raw_data;
        uint8_t checksum;
    } Measurement_t;

    class Sensor
    {
    public:
        virtual float newtons(void) = 0;
        virtual int32_t raw_data(void) = 0;
        virtual void update(void) = 0;
    };

    class Plate
    {
    public:
        explicit Plate(Sensor& sensor);
        Measurement_t read(void);
        void update(void);
        void transmit(void);

    private:
        uint8_t checksum_(const uint8_t* data, uint16_t length);
        Sensor& sensor_;
        int32_t reading_ = 0L;
    };
}
#endif