#include <i2c_driver_wire.h>

#include "ArduinoUtils.h"
// #include "application.h"

const uint8_t I2C_ADDRESS = 0x11;
const uint8_t LED_PIN = 13;
// Newton::Application app;

void setup(void)
{
    // app.setup();
    // initialise i2c comms...
    Wire.begin(I2C_ADDRESS);
    // Wire.onReceive([](int n){app.handle_received(n);});
    // Wire.onRequest([](){app.handle_requested();});
    pinMode(LED_PIN, OUTPUT);
}

void loop(void)
{
    // app.loop();
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(200);
}
