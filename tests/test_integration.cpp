#include <Wire.h>

#include "buffer.h"
#include "logging.h"
#include "node.h"
#include "sensor_stub.h"
#include "serial.h"

const uint32_t BAUD_RATE = 115200;
const uint8_t I2C_ADDRESS = 0x11;

FakeForceSensor* sensor;
Newton::ArduinoSerialHandle* serial_handle;
Newton::Node* node;

Newton::FifoBuffer tx_queue;
Newton::FifoBuffer rx_queue;

void handle_received(int n_bytes)
{
    Serial.print("Received ");
    Serial.print(n_bytes);
    Serial.println(" bytes!");
    // queue the data in the receive buffer...
    for (int i = 0; i < n_bytes; ++i)
    {
        rx_queue.push(Wire.read());
    }
    // notify consumers...
    serial_handle->notify();
}

void handle_requested()
{
    Serial.println("Data requested!");
    // dispatch any data queued in the transmit buffer...
    while (!tx_queue.empty())
    {
        uint8_t to_write;
        tx_queue.pop(&to_write);
        Wire.write(to_write);
    }
}

void setup(void)
{
    Serial.begin(BAUD_RATE);
    LOG_DEBUG("Setting up...");
    sensor = new FakeForceSensor;
    serial_handle = new Newton::ArduinoSerialHandle(tx_queue, rx_queue);
    node = new Newton::Node(*serial_handle, *sensor);

    sensor->begin();
    // initialise i2c comms...
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(handle_received);
    Wire.onRequest(handle_requested);
    while (Wire.available())
    {
        Wire.read();
    }
}

void loop(void)
{
    node->update();
    // Inject some junk data to demonstrate everything is working. We should
    // see a monotonic increase in the signal with the clock...
    sensor->raw_data(millis());
    delay(50);
}
