#ifndef NODE_H
#define NODE_H

#include "ArduinoUtils.h"
#include "interfaces.h"

namespace Newton {

    class Sensor
    {
    public:
        Sensor(void);
        virtual void begin(void) = 0;
        virtual int32_t raw_data(void) const = 0;
        virtual void update(void) = 0;
        float force(void) const;
        void calibrate(Calibration_t& calibration);
        Calibration_t calibration(void) const;

    protected:
        int32_t raw_data_ = 0L;
        Calibration_t calibration_;
    };

    class Node
    {
    public:
        explicit Node(Sensor& sensor);
        void begin(void);
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