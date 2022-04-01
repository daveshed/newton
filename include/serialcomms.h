#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H
#include "ArduinoUtils.h"
#include "node.h"

// facade? that lets commands do their stuff
class CommandInterface {
public:
    explicit CommandInterface(Newton::Sensor& sensor);
    // void transmit(const uint8_t* buf, size_t n);
    void update(void);
private:
    Newton::Sensor& sensor_;
};

class Command {
public:
    void execute();
};
#endif