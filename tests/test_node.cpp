// stdlib
#include <cmath>
// test dependencies
#include "CppUTest/TestHarness.h"
#include "mocks.h"
// code under test
#include "node.h"
#include "Arduino.h"

#define TOLERANCE 0.001

TEST_GROUP(ForcePlateTestGroup)
{
    void setup()
    {}

    void teardown()
    {
        serial_handle.reset();
    }
};

TEST(ForcePlateTestGroup, TestCheckSumNonOverFlowing)
{
    FakeSensor sensor;
    millis(1UL);
    sensor.raw_data(1L);
    Newton::Node node(sensor);
    node.update();
    Newton::Measurement_t reading = node.read();
    CHECK(2U == reading.checksum);
}

TEST(ForcePlateTestGroup, TestChecksumOverflowing)
{
    FakeSensor sensor;
    millis(1UL);
    sensor.raw_data(255L);
    Newton::Node node(sensor);
    node.update();
    Newton::Measurement_t reading = node.read();
    CHECK(0U == reading.checksum);
}

TEST(ForcePlateTestGroup, TestCorrectDataWrittenToSerial)
{
    FakeSensor sensor;
    millis(1UL);
    sensor.raw_data(1L);
    Newton::Node node(sensor);
    node.update();
    node.transmit();
    // compare against data written to serial transmit buffer...
    const Newton::Measurement_t* actual =
        (const Newton::Measurement_t*)serial_handle.tx.data();
    CHECK(1UL == actual->timestamp);
    CHECK(1L == actual->raw_data);
    // Calibration should be 0 on init. Expect 0 N.
    CHECK(fabs(0.0 - actual->force) < TOLERANCE);
}

TEST(ForcePlateTestGroup, TestSensorCalibrates)
{
    FakeSensor sensor;
    Newton::Calibration_t calib{3.0, 4.0};
    sensor.calibrate(calib);
    sensor.raw_data(1.0);
    sensor.update();
    // basic linear calibration...
    // force = raw_data * slope + intercept
    CHECK(fabs(7.0 - sensor.force()) < TOLERANCE);
}
