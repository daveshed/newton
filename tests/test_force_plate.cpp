// stdlib
#include <cmath>
// test dependencies
#include "CppUTest/TestHarness.h"
#include "mocks.h"
// code under test
#include "forcesensor.h"
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
    ForceSensor::Plate force_plate(sensor);
    force_plate.update();
    ForceSensor::Measurement_t reading = force_plate.read();
    CHECK(2U == reading.checksum);
}

TEST(ForcePlateTestGroup, TestChecksumOverflowing)
{
    FakeSensor sensor;
    millis(1UL);
    sensor.raw_data(255L);
    ForceSensor::Plate force_plate(sensor);
    force_plate.update();
    ForceSensor::Measurement_t reading = force_plate.read();
    CHECK(0U == reading.checksum);
}

TEST(ForcePlateTestGroup, TestCorrectDataWrittenToSerial)
{
    FakeSensor sensor;
    millis(1UL);
    sensor.raw_data(1L);
    ForceSensor::Plate force_plate(sensor);
    force_plate.update();
    force_plate.transmit();
    // compare against data written to serial transmit buffer...
    const ForceSensor::Measurement_t* actual =
        (const ForceSensor::Measurement_t*)serial_handle.tx.data();
    CHECK(1UL == actual->timestamp);
    CHECK(1L == actual->raw_data);
    // Calibration should be 0 on init. Expect 0 N.
    CHECK(fabs(0.0 - actual->newtons) < TOLERANCE);
}

TEST(ForcePlateTestGroup, TestSensorCalibrates)
{
    FakeSensor sensor;
    ForceSensor::Calibration_t calib{3.0, 4.0};
    sensor.calibrate(calib);
    sensor.raw_data(1.0);
    sensor.update();
    // basic linear calibration...
    // force = raw_data * slope + intercept
    CHECK(fabs(7.0 - sensor.newtons()) < TOLERANCE);
}
