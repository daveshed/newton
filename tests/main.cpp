// code under test...
#include "forcesensor.h"
// test dependencies...
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "mocks.h"

TEST_GROUP(BasicTests)
{};

TEST(BasicTests, TestCheckSumNonOverflowing)
{
    FakeLoadCell cell;
    set_fake_millis(1UL);
    cell.set_channel_a(1L);
    cell.set_channel_b(1L);
    ForceSensor sensor(cell);
    ForceMeasurement_t reading = sensor.read();
    CHECK(3U == reading.checksum);
}

TEST(BasicTests, TestChecksumOverflowing)
{
    FakeLoadCell cell;
    set_fake_millis(1UL);
    cell.set_channel_a(255L);
    cell.set_channel_b(1L);
    ForceSensor sensor(cell);
    ForceMeasurement_t reading = sensor.read();
    CHECK(1U == reading.checksum);
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

int main(int argc, const char* argv[])
{
    return RUN_ALL_TESTS(argc, argv);
}