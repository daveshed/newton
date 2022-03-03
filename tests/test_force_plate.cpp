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
    FakeSensor sensor_x, sensor_y, sensor_z;
    millis(1UL);
    sensor_x.raw_data(1L);
    sensor_y.raw_data(1L);
    sensor_z.raw_data(1L);
    ForceSensor::Plate force_plate(sensor_x, sensor_y, sensor_z);
    force_plate.update();
    ForceSensor::Measurement_t reading = force_plate.read();
    CHECK(4U == reading.checksum);
}

TEST(BasicTests, TestChecksumOverflowing)
{
    FakeSensor sensor_x, sensor_y, sensor_z;
    millis(1UL);
    sensor_x.raw_data(255L);
    ForceSensor::Plate force_plate(sensor_x, sensor_y, sensor_z);
    force_plate.update();
    ForceSensor::Measurement_t reading = force_plate.read();
    CHECK(0U == reading.checksum);
}

void test_correct_data_written_to_serial(void)
{
    serial_handle.reset();
    FakeSensor sensor_x, sensor_y, sensor_z;
    millis(1UL);
    sensor_x.raw_data(1L);
    sensor_x.newtons(2.0);
    sensor_y.raw_data(1L);
    sensor_z.raw_data(1L);
    ForceSensor::Plate force_plate(sensor_x, sensor_y, sensor_z);
    force_plate.update();
    force_plate.transmit();
    // compare against data written to serial.
    const ForceSensor::Measurement_t* actual =
        (const ForceSensor::Measurement_t*)serial_handle.buffer();
    assert(1UL == actual->timestamp);
    assert(1L == actual->data_x);
    assert(1L == actual->data_y);
    assert(1L == actual->data_z);
    assert(fabs(2.0 - actual->force_x) < 0.01);
}

int main(int argc, const char* argv[])
{
    return RUN_ALL_TESTS(argc, argv);
}