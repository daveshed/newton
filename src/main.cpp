#include <i2c_driver_wire.h>

#include "ArduinoUtils.h"
#include "application.h"

const uint8_t I2C_ADDRESS = 0x11;
const uint8_t LED_PIN = 13;
Newton::Application app;

void setup(void)
{
    // signal that the app is actually loaded...
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(200);

    // use the hardware serial line
    Serial1.begin(9600);
    delay(1000);
    Serial1.println("YO!!");

    // app.setup();
    // initialise i2c comms...
    // Wire.begin(I2C_ADDRESS);
    // Wire.onReceive([](int n){app.handle_received(n);});
    // Wire.onRequest([](){app.handle_requested();});
}

void loop(void)
{
    // app.loop();
}
