#include <cassert>
// code under test...
#include "../forcesensor.h"
// test dependencies...
#include "mocks.h"

void test_checksum_non_overflowing(void)
{
    FakeLoadCell cell;
    set_fake_millis(1UL);
    cell.set_channel_a(1L);
    cell.set_channel_b(1L);
    ForceSensor sensor(cell);
    ForceMeasurement_t reading = sensor.read();
    assert(3U == reading.checksum);
}

void test_checksum_overflowing(void)
{
    FakeLoadCell cell;
    set_fake_millis(1UL);
    cell.set_channel_a(255L);
    cell.set_channel_b(1L);
    ForceSensor sensor(cell);
    ForceMeasurement_t reading = sensor.read();
    assert(1U == reading.checksum);
}

// void test_correct_data_written_to_serial(void)
// {
//     FakeLoadCell cell;
//     set_fake_millis(20UL);
//     cell.set_channel_a(33L);
//     cell.set_channel_b(1L);
//     ForceSensor sensor(cell);
//     ForceMeasurement_t reading = sensor.read();
//     Serial.write(uint_reading)
// }

int main(int argc, char const *argv[])
{
    test_checksum_non_overflowing();
    test_checksum_overflowing();
    return 0;
}