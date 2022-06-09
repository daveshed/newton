// stdlib
#include <cstdint>
#include <iostream>
#include <memory>
// third-party
#include <boost/python.hpp>
// internal
#include "hcomms.h"
#include "i2c_serial.h"

using namespace boost::python;

class InterfaceWrapper {
public:
    InterfaceWrapper(uint8_t address)
    : serial_handle_(1, address)
    , host_(serial_handle_)
    { }
    void calibrate(float slope, float intercept) {
        Newton::Calibration_t calibration = {slope, intercept};
        host_.calibrate(calibration);
    }
    Newton::Measurement_t get_reading(void) {
        return host_.get_reading();
    }
private:
    HostComms::I2cSerial serial_handle_;
    Newton::HostInterface host_;
};

// Build a shared object with this exact name
BOOST_PYTHON_MODULE(hcomms_ext)
{
    class_<Newton::Measurement_t>("Measurement", no_init)
        .def_readonly("timestamp", &Newton::Measurement_t::timestamp)
        .def_readonly("force", &Newton::Measurement_t::force)
        .def_readonly("raw_data", &Newton::Measurement_t::raw_data)
        .def_readonly("checksum", &Newton::Measurement_t::checksum)
    ;

    class_<InterfaceWrapper, boost::noncopyable>("HostInterface", init<uint8_t>())
        .def("calibrate", &InterfaceWrapper::calibrate)
        .def("get_reading", &InterfaceWrapper::get_reading)
    ;
}
