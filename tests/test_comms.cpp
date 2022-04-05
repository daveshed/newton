#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <iostream>
// test dependencies
#include "CppUTest/TestHarness.h"
#include "mocks.h"
#include "sensor_stub.h"
// code under test
#include "serialcomms.h"

#define TOLERANCE  0.001

class FakeSerial : public Newton::Serial {
public:
    FakeSerial(std::deque<uint8_t>& tx, std::deque<uint8_t>& rx)
        : tx_(tx), rx_(rx) {};

    void transmit(uint8_t to_transmit)
    {
        tx_.push_back(to_transmit);
    }

    void transmit(const uint8_t* to_transmit, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            transmit(to_transmit[i]);
        }
    }

    uint8_t receive(void)
    {
        std::cout << "getting data" << std::endl;
        assert(rx_.size());
        uint8_t result = rx_.front();
        rx_.pop_front();
        return result;
    }

    void receive(uint8_t* result, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            result[i] = receive();
        }
    }
private:
    std::deque<uint8_t>& tx_;
    std::deque<uint8_t>& rx_;
};


TEST_GROUP(SerialCommsTestGroup)
{
    void setup()
    {}

    void teardown()
    {
        serial_handle.reset();
        // host_rx.clear();
        // target_rx.clear();
    }
};

TEST(SerialCommsTestGroup, TestCalibratesFromCommand)
{
    // create endpoints and their buffers...
    std::deque<uint8_t> target_rx;
    std::deque<uint8_t> host_rx;
    //                            tx         rx
    auto host_serial = FakeSerial(target_rx, host_rx);
    Newton::HostInterface host(host_serial);
    //                              tx       rx
    auto target_serial = FakeSerial(host_rx, target_rx);
    FakeForceSensor sensor;
    Newton::TargetInterface target(target_serial, sensor);
    // issue the command...
    Newton::Calibration_t expected = {2.0, 3.0};
    host.calibrate(expected);
    target.update();
    // verify the data...
    DOUBLES_EQUAL(
        expected.slope,
        target.sensor().calibration().slope,
        TOLERANCE);
    DOUBLES_EQUAL(
        expected.intercept,
        target.sensor().calibration().intercept,
        TOLERANCE);
}

TEST(SerialCommsTestGroup, TestReadsFromCommand)
{}