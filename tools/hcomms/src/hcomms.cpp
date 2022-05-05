// stdlib
#include <iostream>
// third-party
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
// internal
#include "hostcomms.h"
#include "i2c_serial.h"

char const* greet()
{
    auto serial_handle = HostComms::I2cSerial(1, 0x11);
    auto host = Newton::HostInterface(serial_handle);
    Newton::Measurement_t measurement = host.get_reading();
    std::cout << measurement.raw_data << std::endl;
    return "hello, world";
}

// Build a shared object with this exact name
BOOST_PYTHON_MODULE(hcomms_ext)
{
    using namespace boost::python;
    def("greet", greet);
}
