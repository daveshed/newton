#include <cassert>
#include <cmath>
#include <vector>
// code under test...
#include "forcesensor.h"
#include "Arduino.h"
// test dependencies...
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "mocks.h"


TEST_GROUP(BasicTests)
{};

TEST(BasicTests, TestCheckSumNonOverflowing)
{
    FakeSensor sensor;
    millis(1UL);
    sensor.raw_data(1L);
    ForceSensor::Plate force_plate(sensor);
    force_plate.update();
    ForceSensor::Measurement_t reading = force_plate.read();
    CHECK(2U == reading.checksum);
}

TEST(BasicTests, TestChecksumOverflowing)
{
    FakeSensor sensor;
    millis(1UL);
    sensor.raw_data(255L);
    ForceSensor::Plate force_plate(sensor);
    force_plate.update();
    ForceSensor::Measurement_t reading = force_plate.read();
    CHECK(0U == reading.checksum);
}

void test_correct_data_written_to_serial(void)
{
    serial_handle.reset();
    FakeSensor sensor;
    millis(1UL);
    sensor.raw_data(1L);
    sensor.newtons(2.0);
    ForceSensor::Plate force_plate(sensor);
    force_plate.update();
    force_plate.transmit();
    // compare against data written to serial.
    const ForceSensor::Measurement_t* actual =
        (const ForceSensor::Measurement_t*)serial_handle.buffer();
    assert(1UL == actual->timestamp);
    assert(1L == actual->raw_data);
    assert(fabs(2.0 - actual->newtons) < 0.01);
}

int main(int argc, const char* argv[])
{
    return RUN_ALL_TESTS(argc, argv);
}