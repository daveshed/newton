// real sensor implementation
#include "HX711.h"
#include "node.h"
#include "sensor.h"
// Any digital outputs can be used here. Chip is read by bit-banging these
// pins. It does not use any interrupts or standard comms protocols eg spi or
// i2c. So far tested 2&3, 4&5 and 6&7.
const int SENSOR_DOUT_PIN = 2;
const int SENSOR_SCK_PIN = 3;

// note that changing gain/input takes time.
enum Gain {
    CH_A_HIGH_GAIN = 128, // default
    CH_B_LOW_GAIN = 32,
    CH_A_MID_GAIN = 64,
};

Newton::Sensor* Newton::make_force_sensor(void)
{
    return new HX711ForceSensor();
}

HX711ForceSensor::HX711ForceSensor()
: device_()
{}

void HX711ForceSensor::begin(void)
{
    device_.begin(SENSOR_DOUT_PIN, SENSOR_SCK_PIN, CH_A_HIGH_GAIN);
}

void HX711ForceSensor::update(void)
{
    raw_data_ = device_.read();
}

int32_t HX711ForceSensor::raw_data(void) const
{
    return raw_data_;
}

void HX711ForceSensor::raw_data(int32_t value)
{
    // cannot set the raw data in the real device
    return;
}
