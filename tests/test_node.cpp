#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <cmath>
// third-party
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
// code under test
#include "interfaces.h"
#include "mocks.h"
#include "node.h"

#define TOLERANCE      (0.001f)
#define MAKE_PACKET(type, payload_size, payload_ptr) {\
                                                         { \
                                                             static_cast<uint8_t>(type), \
                                                             payload_size, \
                                                         }, \
                                                         (uint8_t*)payload_ptr \
                                                     }
// commands
#define GET_READING_COMMAND          MAKE_PACKET(Newton::Command::GET_READING, 0U, nullptr)
#define READ_STORED_COMMAND          MAKE_PACKET(Newton::Command::READ_STORED, 0U, nullptr)
#define CALIRBRATE_COMMAND(x)        MAKE_PACKET(\
    Newton::Command::CALIBRATE, \
    sizeof(Newton::Calibration_t), \
    (uint8_t*)&x \
)

// responses
#define CALIBRATE_OK_RESPONSE        MAKE_PACKET(Newton::Response::CALIBRATE_OK, 0U, nullptr)
#define NO_READING_RESPONSE          MAKE_PACKET(Newton::Response::NO_READINGS, 0U, nullptr)
#define HAS_READINGS_RESPONSE(x, y)  MAKE_PACKET(\
    Newton::Response::HAS_READINGS, \
    x, \
    (uint8_t*)&y \
)
#define READ_OK_RESPONSE(x)          MAKE_PACKET(\
    Newton::Response::READ_OK, \
    sizeof(Newton::Measurement_t), \
    (uint8_t*)&x \
)
#define BUSY_RESPONSE                MAKE_PACKET(Newton::Response::BUSY, 0U, nullptr)

class MockSensor : public Newton::Sensor {
public:
    MockSensor(int32_t& raw_data, float& force)
    : raw_data_(raw_data)
    , force_(force)
    {}
    void begin(void) override {
        // this is called separately. node is not responsible for this.
    }
    int32_t raw_data(void) const override {
        mock().actualCall("raw_data").onObject(this);
        return raw_data_;
    }
    void raw_data(int32_t value) override {
        // nobody should call this. It's a mock method.
        (void)value;
        assert(false);
    }
    void update(void) override {
        mock().actualCall("update").onObject(this);
    }
    float force(void) const override {
        mock().actualCall("force").onObject(this);
        return force_;
    }
private:
    int32_t& raw_data_;
    float& force_;
};

std::string as_hex_string(uint8_t value)
{
    char buf[5] = {'\0'};
    sprintf(buf, "0x%02X", value);
    std::string result{buf};
    return result;
}

// comparison operators for asserts.
bool operator==(const Newton::Measurement_t& lhs, const Newton::Measurement_t& rhs)
{
    return lhs.timestamp == rhs.timestamp
        && (fabs(lhs.force - rhs.force) < 1.0E-6)
        && lhs.raw_data == rhs.raw_data
        && lhs.checksum == rhs.checksum;
}

bool operator!=(const Newton::Measurement_t& lhs, const Newton::Measurement_t& rhs)
{
    return !(lhs == rhs);
}

bool operator==(const Newton::Calibration_t& lhs, const Newton::Calibration_t& rhs)
{
    return (fabs(lhs.slope - rhs.slope) < 1.0E-6)
        && (fabs(lhs.intercept - rhs.intercept) < 1.0E-6);
}

bool operator!=(const Newton::Calibration_t& lhs, const Newton::Calibration_t& rhs)
{
    return !(lhs == rhs);
}

bool operator==(const Newton::ResponsePacket::Header& lhs, const Newton::ResponsePacket::Header& rhs)
{
    return lhs.type == rhs.type && lhs.payload_size == rhs.payload_size;
}

bool operator!=(const Newton::ResponsePacket::Header& lhs, const Newton::ResponsePacket::Header& rhs)
{
    return !(lhs == rhs);
}

bool operator==(const Newton::ResponsePacket& lhs, const Newton::ResponsePacket& rhs)
{
    if (lhs.header != rhs.header)
    {
        return false;
    }
    if (lhs.header.type == static_cast<uint8_t>(Newton::Response::READ_OK))
    {
        return *(Newton::Measurement_t*)lhs.payload == *(Newton::Measurement_t*)rhs.payload;
    }
    if (lhs.header.type == static_cast<uint8_t>(Newton::Response::HAS_READINGS))
    {
        // compare readings one by one...
        int n_measurements = lhs.header.payload_size / sizeof(Newton::Measurement_t);
        for (int i = 0; i < n_measurements; ++i)
        {
            if (
                ((Newton::Measurement_t*)lhs.payload)[i]
                != ((Newton::Measurement_t*)rhs.payload)[i]
            )
            {
                return false;
            }
        }
        // all measurements match
        return true;
    }
    // other responses have no payload to verify
    assert(!lhs.header.payload_size);
    assert(!rhs.header.payload_size);
    return true;
}

bool operator!=(const Newton::ResponsePacket& lhs, const Newton::ResponsePacket& rhs)
{
    return !(lhs == rhs);
}

SimpleString StringFrom(const Newton::Calibration_t calibration)
{
    (void)calibration;
    return SimpleString("Calibration_t(TBD)");
}

SimpleString StringFrom(const Newton::ResponsePacket::Header& header)
{
    std::string response_names[static_cast<int>(Newton::Response::MAX)] = {
        "SUCCESS",
        "HAS_READINGS",
        "NO_READINGS",
        "BUSY",
    };
    CHECK(header.type < static_cast<int>(Newton::Response::MAX));  // unknown response
    std::string result =
        "ResponsePacket::Header(type="
        + response_names[header.type]
        + ", size="
        + as_hex_string(header.payload_size)
        + ")";
    return SimpleString(result.c_str());
}

SimpleString StringFrom(const Newton::ResponsePacket& packet)
{
    auto result = SimpleString();
    result += SimpleString("ResponsePacket(header=");
    result += StringFrom(packet.header);
    result += SimpleString(", payload=TBD");
    return result;
}

TEST_GROUP(SerialCommsTestGroup)
{
    Newton::FifoBuffer<uint8_t> rx;
    Newton::FifoBuffer<uint8_t> tx;
    const Newton::Measurement_t initial_reading = {
        1234UL,
        2.345,
        5678UL,
        0U,
    };
    int32_t raw_data;
    float force;
    MockSensor sensor = MockSensor(raw_data, force);
    Newton::Node node =
        Newton::Node(tx, rx, sensor);

    void clear_buffers()
    {
        rx.clear();
        tx.clear();
    }

    void reset_data()
    {
        raw_data = initial_reading.raw_data;
        force = initial_reading.force;
        millis(initial_reading.timestamp);
    }

    void issue_command(Newton::CommandPacket* packet)
    {
        // command packet is pushed onto the device receive buffer...
        CHECK(rx.push(packet->header.type) == 0);
        CHECK(rx.push(packet->header.payload_size) == 0);
        for (size_t i = 0; i < packet->header.payload_size; ++i)
        {
            CHECK(rx.push(packet->payload[i]) == 0);
        }
    }

    /*
      Keep updating until we get the response from the code under test. If it doesn't arrive in
      the specified number of updates, it will throw an assert.
    */
    void read_response(Newton::ResponsePacket *const response, int updates)
    {
        int count = 0;
        // Wait for the header...
        while (true)
        {
            CHECK(count < updates);
            if (tx.size() >= sizeof(Newton::ResponsePacket::Header))
            {
                break;
            }
            node.update();
            count++;
        }
        // Read the header...
        tx.pop((uint8_t*)&response->header, sizeof(Newton::ResponsePacket::Header));
        // Read out the payload as it arrives...
        Newton::payload_size_t bytes_read = 0;
        Newton::payload_size_t total = response->header.payload_size;
        while (true)
        {
            CHECK(count < updates);
            if (tx.size())
            {
                Newton::payload_size_t remaining = total - bytes_read;
                // read the whole buffer unless it's bigger than the remaining data to read...
                Newton::payload_size_t to_read = tx.size() > remaining ? remaining : tx.size();
                tx.pop(response->payload + bytes_read, to_read);
                bytes_read += to_read;
            }
            if (bytes_read == total)
            {
                break;
            }
            node.update();
            count++;
        }
    }

    void setup()
    {
        mock().disable();
        reset_data();
        // ensure that measurements are up to date
        node.update();
    }

    void teardown()
    {
        clear_buffers();
        mock().clear();
    }
};

TEST(SerialCommsTestGroup, TestCalibratesFromCommand)
{
    // 1. issue the command...
    Newton::Calibration_t const expected_calibration = {2.0, 3.0};
    Newton::CommandPacket command_packet = CALIRBRATE_COMMAND(expected_calibration);
    issue_command(&command_packet);
    // 2. update the node so the command is handled...
    Newton::ResponsePacket actual_response;
    read_response(&actual_response, 10);
    // 3. verify the data...
    Newton::Calibration_t actual_calibration = node.sensor().calibration();
    CHECK_EQUAL(expected_calibration, actual_calibration);
    // 4. should notify that the command was successful...
    Newton::ResponsePacket expected_response = CALIBRATE_OK_RESPONSE;
    CHECK_EQUAL(expected_response, actual_response);
}

TEST(SerialCommsTestGroup, TestReadsDataFromCommandShouldQueryMockSensor)
{
    // 1a. Queue expectations on the mock sensor that should be read...
    mock().enable();
    // The node will update twice - Once to determine the command type, and another to actually
    // process it...
    mock().expectOneCall("update").onObject(&sensor);
    mock().expectOneCall("force").onObject(&sensor);
    mock().expectOneCall("raw_data").onObject(&sensor);
    mock().expectOneCall("update").onObject(&sensor);
    mock().expectOneCall("force").onObject(&sensor);
    mock().expectOneCall("raw_data").onObject(&sensor);
    mock().ignoreOtherCalls();
    // 1b. issue the command...
    Newton::CommandPacket command_packet = GET_READING_COMMAND;
    issue_command(&command_packet);
    // 2. update the node so the command is handled...
    Newton::ResponsePacket actual_response;
    read_response(&actual_response, 4);
    mock().checkExpectations();
    mock().disable();
    // 3. Validate the response...
    Newton::Measurement_t expected_measurement = {
        millis(),
        force,
        raw_data,
        0,  // FIXME: need the checksum
    };
    Newton::ResponsePacket expected_response = READ_OK_RESPONSE(expected_measurement);
    CHECK_EQUAL(expected_response, actual_response);
}
#if 0

TEST(SerialCommsTestGroup, TestForcesBelowThresholdNoRecordedDataAvailable)
{
    // Only when forces above a given threshold are detected will the node start
    // recording data. Once this happens, data will be stored in a buffer for
    // retrieval by the host. In this test, the force remains below the
    // threshold so there is no data available.
    // 1. issue the command...
    Newton::CommandPacket command_packet = READ_STORED_COMMAND;
    issue_command(&command_packet);
    Newton::ResponsePacket actual_response;
    read_response(&actual_response, 10);
    // 2. validate response - no readings should be stored...
    Newton::ResponsePacket empty_storage_response = NO_READING_RESPONSE;
    CHECK_EQUAL(empty_storage_response, actual_response);
    // 3. the force changes but remains below threshold...
    force = -1.0 * (Newton::Node::threshold - TOLERANCE);
    // keep increasing the force until just below the threshold...
    while (true)
    {
        node.update();
        force += 1.0f;
        if (fabs(force) < Newton::Node::threshold)
        {
            break;
        }
    }
    // 4. query the storage - should still be no data...
    issue_command(&command_packet);
    read_response(&actual_response, 10);
    CHECK_EQUAL(empty_storage_response, actual_response);
}

TEST(SerialCommsTestGroup, TestForcesAboveThresholdShouldRecordData)
{
    // 1. the force goes above the threshold and stays there for a bit...
    force = Newton::Node::threshold + TOLERANCE;
    const int n_measurements = 5;
    Newton::Measurement_t expected_measurements[n_measurements];
    for (int i = 0; i < n_measurements; ++i)
    {
        // inject data...
        force += 1.0f;
        millis(millis() + i);
        raw_data += 1L;
        // and store it for comparison later..
        expected_measurements[i] = {millis(), force, raw_data, 0U};
        // flush...
        node.update();
    }
    // ...then returns to below the threshold.
    force = Newton::Node::threshold - TOLERANCE;
    // Any more calls to update should mean that no more data will be recorded (ie. the number of
    // measurements should not change)...
    node.update();
    node.update();
    // 2. Query the storage there should be some readings. We will need to update so that each
    //    reading can be loaded into the buffer...
    Newton::CommandPacket command_packet = READ_STORED_COMMAND;
    issue_command(&command_packet);
    const size_t payload_size = n_measurements * sizeof(Newton::Measurement_t);
    Newton::Measurement_t actual_measurements[n_measurements];
    Newton::ResponsePacket actual;
    actual.payload = (uint8_t*)actual_measurements;
    read_response(&actual, 10);
    // 3. Compare and Assert...
    Newton::ResponsePacket expected = HAS_READINGS_RESPONSE(payload_size, expected_measurements);
    CHECK_EQUAL(expected, actual);
}

TEST(SerialCommsTestGroup, TestRejectsReadStoredCommandWhenRecording)
{
    // 1. trigger data recording...
    force = Newton::Node::threshold + TOLERANCE;
    node.update();
    node.update();
    // 2. When we try to retrieve measurements, the request should be rejected...
    Newton::CommandPacket command_packet = READ_STORED_COMMAND;
    issue_command(&command_packet);
    Newton::ResponsePacket expected_response = BUSY_RESPONSE;
    Newton::ResponsePacket actual_response;
    read_response(&actual_response, 10);
    // 3. Compare and assert...
    CHECK_EQUAL(expected_response, actual_response);
}
#endif