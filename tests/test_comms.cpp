#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
// test dependencies
#include "CppUTest/TestHarness.h"
#include "mocks.h"
#include "sensor_stub.h"
// code under test
#include "hostcomms.h"
#include "node.h"

#define TOLERANCE  0.001

class Buffer {
public:
    Buffer(void) : callback_(nullptr), deque_() {};
    void push(uint8_t val) {
        printf("Pushing <0x%02x>...\n", val);
        deque_.push_back(val);
        if (callback_)
        {
            printf("Calling callback at %p...\n", callback_);
            (*callback_)();
        }
        print();
    };

    uint8_t pop(void) {
        printf("Popping...\n");
        assert(deque_.size());
        uint8_t result = deque_.front();
        deque_.pop_front();
        print();
        return result;
    };

    void clear(void) {
        deque_.clear();
    }

    size_t size(void)
    {
        return deque_.size();
    }

    void register_callback(Newton::SerialDataCallback* cb)
    {
        callback_ = cb;
        printf("Registered callback %p.\n", callback_);
    };
private:
    void print(void)
    {
        printf("buffer contains: ");
        for (auto& element : deque_)
        {
            printf("0x%02x ", element);
        }
        putchar('\n');
    }
    Newton::SerialDataCallback* callback_;
    std::deque<uint8_t> deque_;
};

class FakeSerial : public Newton::SerialHandle {
public:
    FakeSerial(Buffer& tx, Buffer& rx)
        : tx_(tx), rx_(rx) {};

    void transmit(uint8_t to_transmit) override
    {
        tx_.push(to_transmit);
    }

    void transmit(const uint8_t* to_transmit, size_t n) override
    {
        printf("Sending data...");
        for (size_t i = 0; i < n; ++i)
        {
            transmit(to_transmit[i]);
        }
    }

    uint8_t receive(void) override
    {
        return rx_.pop();
    }

    void receive(uint8_t* result, size_t n) override
    {
        for (size_t i = 0; i < n; ++i)
        {
            result[i] = receive();
        }
    }

    size_t available(void) override
    {
        return rx_.size();
    }

    void register_callback(Newton::SerialDataCallback* cb) override
    {
        // call when there is data waiting in the receive buffer.
        rx_.register_callback(cb);
    }

    Buffer& tx_;
    Buffer& rx_;
};


TEST_GROUP(SerialCommsTestGroup)
{
    // create endpoints and their buffers...
    Buffer target_rx;
    Buffer host_rx;
    // create interfaces - they use each other's buffers
    //                                  tx         rx
    FakeSerial host_serial = FakeSerial(target_rx, host_rx);
    Newton::HostInterface host = Newton::HostInterface(host_serial);
    //                                    tx       rx
    FakeSerial target_serial = FakeSerial(host_rx, target_rx);
    FakeForceSensor sensor;
    Newton::Node target =
        Newton::Node(target_serial, sensor);

    void clear_buffers()
    {
        host_rx.clear();
        target_rx.clear();
    }

    void setup()
    {
        // ensure that measurements are up to date
        target.update();
    }

    void teardown()
    {
        clear_buffers();
    }
};

TEST(SerialCommsTestGroup, TestCalibratesFromCommand)
{
    // 1. issue the command...
    Newton::Calibration_t expected = {2.0, 3.0};
    host.calibrate(expected);
    target.update();
    // 2. verify the data...
    Newton::Calibration_t actual = target.sensor().calibration();
    MEMCMP_EQUAL(&expected, &actual, sizeof(Newton::Calibration_t));
}

TEST(SerialCommsTestGroup, TestReadsDataFromCommand)
{
    // 1. Issue request for data...
    Newton::Measurement_t actual = host.get_reading();
    // 2. Validate...
    Newton::Measurement_t expected = {
        millis(),
        sensor.force(),
        sensor.raw_data(),
        0,  // FIXME: need the checksum
    };
    MEMCMP_EQUAL(&expected, &actual, sizeof(Newton::Measurement_t));
}