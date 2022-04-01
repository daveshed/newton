#include "node.h"
#include "sensor_stub.h"

const int BAUD_RATE = 9600;

FakeForceSensor sensor;
Newton::Node node(sensor);

void setup(void)
{
    Serial.begin(BAUD_RATE);
    node.begin();
}

void loop(void)
{
    // Inject some junk data to demonstrate everything is working. We should
    // see a monotonic increase in the signal with the clock...
    sensor.raw_data(millis());
    node.update();
    node.transmit();
}
