#include <cassert>
#include <cmath>

#include "mocks.h"
#include "../serialcomms.h"

#define TOLERANCE  0.001

void test_calibrates_sensor_from_command(void)
{
    const ForceSensor::Calibration_t expected = {2.0, 3.0};
    serial_handle.reset();
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
    const uint8_t command[] = {0};
    // push calibrate command + payload into the receive buffer...
    // serial_handle.rx.push_back(0x0);
    serial_handle.rx.insert(
        serial_handle.rx.end(),
        (const uint8_t*)&expected,
        (const uint8_t*)&expected + sizeof(ForceSensor::Calibration_t));
    serial_handle.print();
    FakeSensor sensor;
    CommandInterface interface(sensor);
    // command should be processed...
    interface.update();
    assert(
        fabs(expected.slope - sensor.calibration().slope) < TOLERANCE);
    assert(
        fabs(expected.intercept - sensor.calibration().intercept) < TOLERANCE);
    assert(serial_handle.rx.empty());
}

void test_bar(void)
{

}

int main(void)
{
    test_calibrates_sensor_from_command();
    test_bar();
    return 0;
}