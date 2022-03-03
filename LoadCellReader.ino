#include "forcesensor.h"
#include "HX711.h"

// Any digital outputs can be used here. Chip is read by bit-banging these
// pins. It does not use any interrupts or standard comms protocols eg spi or
// i2c. So far tested 2&3, 4&5 and 6&7.
const int SENSOR_X_DOUT_PIN = 2;
const int SENSOR_X_SCK_PIN = 3;
const int SENSOR_Y_DOUT_PIN = 4;
const int SENSOR_Y_SCK_PIN = 5;
const int SENSOR_Z_DOUT_PIN = 6;
const int SENSOR_Z_SCK_PIN = 7;
// const int BAUD_RATE = 9600;
const int BAUD_RATE = 19200;

// note that changing gain/input takes time.
enum Gain {
    CH_A_HIGH_GAIN = 128, // default
    CH_B_LOW_GAIN = 32,
    CH_A_MID_GAIN = 64,
};

class HX711ForceSensor : public ForceSensor::Sensor {
public:
    HX711ForceSensor(HX711& device) : device_(device) {};
    float newtons(void) {return 0.0;};
    void update(void) {raw_data_ = device_.read();};
    int32_t raw_data(void) {return raw_data_;};

private:
    HX711& device_;
    int32_t raw_data_ = 0L;
};

HX711 device_x, device_y, device_z;
HX711ForceSensor sensor_x(device_x);
HX711ForceSensor sensor_y(device_y);
HX711ForceSensor sensor_z(device_z);
ForceSensor::Plate force_plate(sensor_x, sensor_y, sensor_z);

void setup(void)
{
    Serial.begin(BAUD_RATE);
    device_x.begin(SENSOR_X_DOUT_PIN, SENSOR_X_SCK_PIN);
    device_x.set_gain(CH_A_HIGH_GAIN);
    device_y.begin(SENSOR_Y_DOUT_PIN, SENSOR_Y_SCK_PIN);
    device_y.set_gain(CH_A_HIGH_GAIN);
    device_z.begin(SENSOR_Z_DOUT_PIN, SENSOR_Z_SCK_PIN);
    device_z.set_gain(CH_A_HIGH_GAIN);
}

void loop(void)
{
    force_plate.update();
    force_plate.transmit();
}
