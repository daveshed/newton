#include "ArduinoUtils.h"
#include <i2c_driver_wire.h>

#include "application.h"
#include "interfaces.h"
#include "logging.h"

const uint32_t BAUD_RATE = 115200;

using namespace Newton;

void Application::setup(void)
{
    Serial.begin(BAUD_RATE);
    LOG_DEBUG("Setting up...");
    sensor_ = make_force_sensor();
    node_ = new Newton::Node(tx_queue_, rx_queue_, *sensor_);
    sensor_->begin();
}

void Application::handle_received(int n_bytes)
{
    LOG_DEBUG("Handling received data...");
    // queue the data in the receive buffer...
    for (int i = 0; i < n_bytes; ++i)
    {
        rx_queue_.push(Wire.read());
    }
}

void Application::handle_requested()
{
    LOG_DEBUG("Data requested!");
    // dispatch any data queued in the transmit buffer...
    while (!tx_queue_.empty())
    {
        uint8_t to_write;
        tx_queue_.pop(&to_write);
        Wire.write(to_write);
    }
}

void Application::loop(void)
{
    node_->update();
    // Inject some junk data to demonstrate everything is working. We should
    // see a monotonic increase in the signal with the clock...
    sensor_->raw_data(millis());
};
