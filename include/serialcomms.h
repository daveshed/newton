#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H
#include "ArduinoUtils.h"
#include "node.h"

namespace Newton {

class Serial {
public:
    virtual void transmit(uint8_t to_transmit) = 0;
    virtual void transmit(const uint8_t* to_transmit, size_t n) = 0;
    virtual uint8_t receive(void) = 0;
    virtual void receive(uint8_t* result, size_t n) = 0;
};

class HostInterface {
public:
    explicit HostInterface(Serial& serial);
    void calibrate(Calibration_t calibration) const;
private:
    Serial& serial_;
};

class TargetInterface {
public:
    explicit TargetInterface(Serial& serial, Sensor& sensor);
    void update(void);
    Sensor& sensor(void) const;
    Serial& serial(void) const;
private:
    Serial& serial_;
    Sensor& sensor_;
};

class CommandBase {
public:
    virtual void execute(TargetInterface&) = 0;
};

class CalibrateCommand : public CommandBase {
public:
    CalibrateCommand(Calibration_t calib);
    void execute(TargetInterface&) override;
private:
    Calibration_t calibration_;
};

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

} // namespace
#endif