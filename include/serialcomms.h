#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H
#include "Arduino.h"
#include "node.h"

// facade? that lets commands do their stuff
class CommandInterface {
public:
    explicit CommandInterface(Newton::Sensor&);
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