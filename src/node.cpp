#include "ArduinoUtils.h"
#include "buffer.h"
#include "interfaces.h"
#include "logging.h"
#include "node.h"

using namespace Newton;

Sensor::Sensor(void)
: calibration_{0, 0}
{}

void Sensor::calibrate(Calibration_t& calibration)
{
    calibration_ = calibration;
}

float Sensor::force(void) const
{
    return (raw_data_ * calibration_.slope) + calibration_.intercept;
}

Calibration_t Sensor::calibration(void) const
{
    return calibration_;
}

Node::Node(FifoBuffer<uint8_t>& tx, FifoBuffer<uint8_t>& rx, Sensor& sensor)
: tx_{tx}
, rx_{rx}
, sensor_{sensor}
, codec_{this}
, reading_{}
{ }

Sensor& Node::sensor(void) const
{
    return sensor_;
}

// should theses just belong to the codec?
Newton::FifoBuffer<uint8_t>& Node::tx(void) const
{
    return tx_;
}

// should theses just belong to the codec?
Newton::FifoBuffer<uint8_t>& Node::rx(void) const
{
    return rx_;
}

Newton::FifoBuffer<Newton::Measurement_t>& Node::buffer(void)
{
    return buffer_;
}

void Node::update(void)
{
    // 1. refresh readings...
    sensor_.update();
    reading_ = {
        millis(),
        sensor().force(),
        sensor().raw_data(),
        0, // FIXME: calculate checksum
    };
    if (is_measuring())
    {
        LOG_DEBUG("Force is above threshold.");
        buffer_.push(reading_);
    }
    // 2. handle serial data...
    codec_.update();
}

Measurement_t Node::reading(void) const
{
    return reading_;
}

bool Node::is_measuring(void) const
{
    return reading_.force > threshold;
}

uint8_t Node::checksum_(const uint8_t* data, uint16_t length)
{
    uint8_t checksum = 0U;
    for (uint16_t i = 0; i < length; i++)
    {
        checksum += data[i];
    }
    return checksum;
}

NodeCodec::NodeCodec(Node* node) : node_{node}, header_{0U, 0U}
{
    handler_ = &NodeCodec::handle_request_;
}

void NodeCodec::update(void)
{
    CALL_MEMBER_FN(this, handler_)();
}

void NodeCodec::handle_calibrate_(void)
{
    LOG_DEBUG("handling calibration request...");
    // read the calibration data from serial when it's arrived...
    if (node_->rx().size() < header_.payload_size)
    {
        return;
    }
    ASSERT(header_.payload_size == sizeof(Calibration_t));
    Calibration_t calibration;
    node_->rx().pop((uint8_t*)&calibration, sizeof(Calibration_t));
    // apply it to the sensor...
    node_->sensor().calibrate(calibration);
    // done
    handler_ = &NodeCodec::handle_request_;
    LOG_DEBUG("Handled calibration request.");
    // send the response...
    send_response_(Response::CALIBRATE_OK, 0U, nullptr);
}

void NodeCodec::handle_get_reading_(void)
{
    LOG_DEBUG("handling get reading request...");
    Measurement_t payload = node_->reading();
    handler_ = &NodeCodec::handle_request_;
    send_response_(Response::READ_OK, sizeof(Measurement_t), (const uint8_t*)&payload);
    LOG_DEBUG("done.");
}

void NodeCodec::handle_read_stored_(void)
{
    LOG_DEBUG("handling read stored request...");
    if (node_->buffer().empty())
    {
        LOG_DEBUG("No stored data...");
        handler_ = &NodeCodec::handle_request_;
        send_response_(Newton::Response::NO_READINGS, 0U, nullptr);
    }
    else if (node_->is_measuring())
    {
        LOG_DEBUG("Busy recording data...");
        handler_ = &NodeCodec::handle_request_;
        send_response_(Newton::Response::BUSY, 0U, nullptr);
    }
    else
    {
        LOG_DEBUG("Returning stored data...");
        size_t payload_size = node_->buffer().size() * sizeof(Measurement_t);
        ResponsePacket::Header header = {
            static_cast<uint8_t>(Newton::Response::HAS_READINGS),
            static_cast<payload_size_t>(payload_size),
        };
        handler_ = &NodeCodec::handle_transmit_stored_payload_;
        send_response_(header);
    }
}

void NodeCodec::handle_transmit_stored_payload_(void)
{
    bool have_readings = node_->buffer().size();
    bool can_transmit = node_->tx().remaining() >= sizeof(Measurement_t);
    if (!have_readings)
    {
        LOG_DEBUG("Done transmitting measurements.");
        handler_ = &NodeCodec::handle_request_;
        return;
    }
    if (!can_transmit)
    {
        // need to wait for space to transmit the data...
        return;
    }
    // Have data and space to transmit...
    Measurement_t to_transmit;
    node_->buffer().pop(&to_transmit);
    ASSERT(node_->tx().push((const uint8_t*)&to_transmit, sizeof(Measurement_t)) == 0);
}

void NodeCodec::handle_request_(void)
{
    // lookup table...
    handler_type handlers[static_cast<int>(Command::MAX)] = {
        &NodeCodec::handle_request_,
        &NodeCodec::handle_calibrate_,
        &NodeCodec::handle_get_reading_,
        &NodeCodec::handle_read_stored_,
    };
    if (node_->rx().size() < sizeof(CommandPacket::Header))
    {
        // nothing to do - waiting for header
        return;
    }
    // Header is here. Let's read it...
    CommandPacket::Header header;
    node_->rx().pop((uint8_t*)&header, sizeof(CommandPacket::Header));
    // execute it on this...
    LOG_DEBUG("Got command.");
    LOG_DEBUG(static_cast<int>(header.type));
    // Find the correct handler and call it to dispatch the data.
    handler_ = handlers[header.type];
    header_ = header;
}

void NodeCodec::send_response_(const Newton::ResponsePacket::Header& header) const
{
    ASSERT(node_->tx().push((const uint8_t*)&header, sizeof(ResponsePacket::Header)) == 0);
}

void NodeCodec::send_response_(
        Newton::Response type, payload_size_t size, const uint8_t* payload) const
{
    ResponsePacket::Header header = {static_cast<uint8_t>(type), size};
    // send header...
    send_response_(header);
    // send payload...
    if (size)
    {
        ASSERT(node_->tx().push(payload, size) == 0);
    }
}
