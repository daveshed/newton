#include "node.h"
#include "sensor_stub.h"
#include "serial.h"

const int BAUD_RATE = 9600;

FakeForceSensor sensor;
Newton::ArduinoSerialHandle serial_handle = Newton::ArduinoSerialHandle();
Newton::Node node(serial_handle, sensor);

void setup(void)
{
    Serial.begin(BAUD_RATE);
    sensor.begin();
}

void loop(void)
{
    // Inject some junk data to demonstrate everything is working. We should
    // see a monotonic increase in the signal with the clock...
    sensor.raw_data(millis());
    node.update();
}

// notify listeners via this interrupt...
void serialEvent(void)
{
    (*serial_handle.callback())();
}