#include "node.h"
#include "sensor_stub.h"
#include "serial.h"

#define LED_SETUP()   (pinMode(LED_BUILTIN, OUTPUT))
#define LED_ON()      (digitalWrite(LED_BUILTIN, HIGH))
#define LED_OFF()     (digitalWrite(LED_BUILTIN, LOW))
#define IS_LED_ON()   (digitalRead(LED_BUILTIN))

const int BAUD_RATE = 9600;

FakeForceSensor* sensor;
Newton::ArduinoSerialHandle* serial_handle;
Newton::Node* node;

void setup(void)
{
    sensor = new FakeForceSensor;
    serial_handle = new Newton::ArduinoSerialHandle();
    node = new Newton::Node(*serial_handle, *sensor);

    Serial.begin(BAUD_RATE);
    sensor->begin();
    node->update();
    LED_SETUP();
    LED_OFF();
}

void loop(void)
{
    // Inject some junk data to demonstrate everything is working. We should
    // see a monotonic increase in the signal with the clock...
    sensor->raw_data(millis());
    node->update();
}

// notify listeners via this interrupt...
void serialEvent(void)
{
    if (IS_LED_ON())
    {
        LED_OFF();
    }
    else
    {
        LED_ON();
    }
    serial_handle->notify();
}
