#include "HX711.h"

HX711 loadcell;

// Any digital outputs can be used here. Chip is read by bit-banging these
// pins. It does not use any interrupts or standard comms protocols eg spi or
// i2c. So far tested 2&3, 4&5 and 6&7.
const int LOADCELL_DOUT_PIN = 6;
const int LOADCELL_SCK_PIN = 7;
const int BAUD_RATE = 19200;

enum Gain {
    CH_A_HIGH_GAIN = 128,
    CH_B_LOW_GAIN = 32,
    CH_A_MID_GAIN = 64,
};

struct MyData {
    uint32_t timestamp;
    int32_t channel_a;
    int32_t channel_b;
};

#if 0
void read_non_blocking(uint16_t timeout)
{
    if (loadcell.wait_ready_timeout(timeout))
    {
        int32_t reading = loadcell.read();
        Serial.println(reading);
    }
    else
    {
        Serial.println("Device not found.");
    }
}

uint16_t count_readings(uint32_t interval_ms)
{
    uint32_t deadline = millis() + interval_ms;
    uint16_t counter = 0U;
    while (millis() < deadline)
    {
        loadcell.read();
        counter += 1U;
    }
    return counter;
}
#endif

MyData read_both_channels(void)
{
    MyData result;
    result.timestamp = millis();
    result.channel_a = loadcell.read();
    result.channel_b = 0L;
    return result;
}

void write_to_serial(const MyData& data)
{
    Serial.write((uint8_t const* const)&data, sizeof(MyData));
}

void setup(void)
{
    Serial.begin(BAUD_RATE);
    loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    loadcell.set_gain(CH_A_HIGH_GAIN);
}

void loop(void)
{
    write_to_serial(read_both_channels());
}
