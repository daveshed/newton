#include <Wire.h>

#include "logging.h"
#include "node.h"
#include "sensor_stub.h"
#include "serial.h"

const uint32_t BAUD_RATE = 115200;
const uint8_t I2C_ADDRESS = 0x11;

FakeForceSensor* sensor;
Newton::ArduinoSerialHandle* serial_handle;
Newton::Node* node;

uint8_t buffer[32];
uint8_t size;;

void handle_received(int count)
{
    Serial.print("Received ");
    Serial.print(count);
    Serial.println(" bytes!");
    // serial_handle->notify();
    size = 0;
    for (int i = 0; i < count; ++i)
    {
        uint8_t received = Wire.read();
        Serial.print(received);
        Serial.print(" ");
        buffer[i] = received;
        size++;
    }
    Serial.println();
}

void handle_requested()
{
    Serial.print("Data requested! Sending ");
    for (int i = 0; i < size; ++i)
    {
        uint8_t to_send = buffer[i];
        Serial.print(to_send);
        Serial.print(" ");
        Wire.write(to_send);
    }
    Serial.println();
}

void setup(void)
{
    Serial.begin(BAUD_RATE);
    LOG_DEBUG("Setting up...");
    // sensor = new FakeForceSensor;
    // serial_handle = new Newton::ArduinoSerialHandle(I2C_ADDRESS);
    // node = new Newton::Node(*serial_handle, *sensor);

    // sensor->begin();
    Wire.begin(0x11);
    Wire.onReceive(handle_received);
    Wire.onRequest(handle_requested);
    while (Wire.available())
    {
        Wire.read();
    }
}

void loop(void)
{
    // node->update();
    // Inject some junk data to demonstrate everything is working. We should
    // see a monotonic increase in the signal with the clock...
    // sensor->raw_data(millis());
    // serial_handle->transmit(0x55);
    delay(10);
}
