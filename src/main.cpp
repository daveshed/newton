#include "node.h"
#include "sensor.h"

const int BAUD_RATE = 9600;

HX711ForceSensor sensor;
Newton::Node node(sensor);

void setup(void)
{
    Serial.begin(BAUD_RATE);
    node.begin();
}

void loop(void)
{
    node.update();
    node.transmit();
}
