// test dependencies
#include "CppUTest/TestHarness.h"
#include "mocks.h"
// code under test
#include "serialcomms.h"

#define TOLERANCE  0.001

TEST_GROUP(SerialCommsTestGroup)
{
    void setup()
    {}

    void teardown()
    {
        serial_handle.reset();
    }

};

TEST(SerialCommsTestGroup, TestCalibratesSensorFromCommand)
{
    const Newton::Calibration_t expected = {2.0, 3.0};
#if 0
    // idea...
    FakeSerialConnection serial;  // flip rx/tx for host and target
    HostInterface host(serial.host);
    TargetInterface target(serial.target);
    // host sends the command...
    ForceSensor::CalibrateCommand command(expected);
    host.send(command);
    // target end should receive it...
    GenericCommand* result;
    target.receive(result);
    // The result should visit the sensor and do what it needs to do...
    sensor.handle(result);
#endif
    // push calibrate command + payload into the receive buffer...
    // serial_handle.rx.push_back(0x0);
    serial_handle.rx.insert(
        serial_handle.rx.end(),
        (const uint8_t*)&expected,
        (const uint8_t*)&expected + sizeof(Newton::Calibration_t));
    serial_handle.print();
    FakeSensor sensor;
    CommandInterface interface(sensor);
    // command should be processed...
    interface.update();
    DOUBLES_EQUAL(
        expected.slope,
        sensor.calibration().slope,
        TOLERANCE);
    DOUBLES_EQUAL(
        expected.intercept,
        sensor.calibration().intercept,
        TOLERANCE);
    CHECK(serial_handle.rx.empty());
}
