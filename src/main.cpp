#include "node.h"
#include "HX711.h"

// Any digital outputs can be used here. Chip is read by bit-banging these
// pins. It does not use any interrupts or standard comms protocols eg spi or
// i2c. So far tested 2&3, 4&5 and 6&7.
const int SENSOR_DOUT_PIN = 2;
const int SENSOR_SCK_PIN = 3;
const int BAUD_RATE = 9600;

// note that changing gain/input takes time.
enum Gain {
    CH_A_HIGH_GAIN = 128, // default
    CH_B_LOW_GAIN = 32,
    CH_A_MID_GAIN = 64,
};

class HX711ForceSensor : public Newton::Sensor {
public:
    HX711ForceSensor(HX711& device) : device_(device) {};
    float force(void) {
        return 0.0;
    };
    void update(void) override {
        raw_data_ = device_.read();
    };
    int32_t raw_data(void) const override {
        return raw_data_;
    };

private:
    HX711& device_;
    int32_t raw_data_ = 0L;
};

HX711 device;
HX711ForceSensor sensor(device);
Newton::Node node(sensor);

void setup(void)
{
    Serial.begin(BAUD_RATE);
    device.begin(SENSOR_DOUT_PIN, SENSOR_SCK_PIN);
    device.set_gain(CH_A_HIGH_GAIN);
}

void loop(void)
{
    node.update();
    node.transmit();
}
