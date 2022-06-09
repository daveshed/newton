#include <Wire.h>

#include "ArduinoUtils.h"
#include "application.h"

const uint8_t I2C_ADDRESS = 0x11;
Newton::Application app;

void setup(void)
{
    app.setup();
    // initialise i2c comms...
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive([](int n){app.handle_received(n);});
    Wire.onRequest([](){app.handle_requested();});
}

void loop(void)
{
    app.loop();
}
