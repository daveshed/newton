#ifndef FORCESENSOR_H
#define FORCESENSOR_H

#include "Arduino.h"  // cstdint

namespace ForceSensor {
    class Sensor;
    class Plate;

    typedef struct __attribute__((packed)) Measurement_s {
        uint32_t timestamp;
        float force_x;
        float force_y;
        float force_z;
        int32_t data_x;
        int32_t data_y;
        int32_t data_z;
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
        explicit Plate(Sensor& x, Sensor& y, Sensor& z);
        Measurement_t read(void);
        void update(void);
        void transmit(void);
        static const uint8_t max_readings = 0xFF;

    private:
        uint8_t checksum_(const uint8_t* data, uint16_t length);
        Sensor& sensor_x_;
        Sensor& sensor_y_;
        Sensor& sensor_z_;
        uint8_t n_readings_ = 0U;
        int32_t raw_data_[max_readings] = {0L};
    };
}
#endif