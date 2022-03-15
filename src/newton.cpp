#include "forcesensor.h"
#include "HX711.h"

// Any digital outputs can be used here. Chip is read by bit-banging these
// pins. It does not use any interrupts or standard comms protocols eg spi or
// i2c. So far tested 2&3, 4&5 and 6&7.
const int LOADCELL_DOUT_PIN = 6;
const int LOADCELL_SCK_PIN = 7;
// const int BAUD_RATE = 9600;
const int BAUD_RATE = 19200;

// note that changing gain/input takes time.
enum Gain {
    CH_A_HIGH_GAIN = 128,
    CH_B_LOW_GAIN = 32,
    CH_A_MID_GAIN = 64,
};


class LoadCellImpl : public LoadCell {
public:
    LoadCellImpl(HX711& device) : device_(device) {};
    int32_t read_channel_a(void) {return device_.read();};
    int32_t read_channel_b(void) {return 0L;};

private:
    HX711& device_;
};

HX711 device;
LoadCellImpl loadcell(device);
ForceSensor sensor(loadcell);

void write_to_serial(const ForceMeasurement_t& data)
{
    // data and checksum written at once...
    Serial.flush();
    Serial.write((const uint8_t*)&data, sizeof(ForceMeasurement_t));
}

void setup(void)
{
    Serial.begin(BAUD_RATE);
    device.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    device.set_gain(CH_A_HIGH_GAIN);
}

void loop(void)
{
    write_to_serial(sensor.read());
}
