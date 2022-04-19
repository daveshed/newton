#include <string>
#include <vector>

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include "CppLinuxSerial/SerialPort.hpp"

#include "hostcomms.h"

using namespace mn;

// implement serial
class ConcreteSerial : public Newton::SerialHandle {
public:
    ConcreteSerial(
        const std::string& device,
        CppLinuxSerial::BaudRate baud)
    : serial_port_(
        device,
        baud,
        CppLinuxSerial::NumDataBits::EIGHT,
        CppLinuxSerial::Parity::NONE,
        CppLinuxSerial::NumStopBits::ONE)
    {
        serial_port_.Open();
    }
    ~ConcreteSerial()
    {
        serial_port_.Close();
    }
    void transmit(uint8_t to_transmit) override
    {
        std::vector<uint8_t> data(to_transmit);
        serial_port_.WriteBinary(data);
    }
    void transmit(const uint8_t* to_transmit, size_t n) override
    {
        std::vector<uint8_t> data(to_transmit[0], to_transmit[(n - 1)]);
        serial_port_.WriteBinary(data);
    }
    uint8_t receive(void) override
    {
        uint8_t result = 0;
        return result;
    }
    void receive(uint8_t* result, size_t n) override
    {
        (void)result;
        (void)n;
    }
    size_t available(void) override
    {
        size_t result = 0;
        return result;
    }
    void register_callback(Newton::SerialDataCallback* callback) override
    {
        (void)callback;
    }
private:
    CppLinuxSerial::SerialPort serial_port_;
};

char const* greet()
{
    auto serial_handle = ConcreteSerial(
        "/dev/ttyACM0",
        CppLinuxSerial::BaudRate::B_9600);

    auto host = Newton::HostInterface(serial_handle);
    host.get_reading();
    // SerialPort serialPort(
    //    "/dev/ttyACM0",
    //    BaudRate::B_9600,
    //    NumDataBits::EIGHT,
    //    Parity::NONE,
    //    NumStopBits::ONE);
    // serialPort.SetTimeout(-1);  // blocking
    // serialPort.Open();
    // serialPort.Close();
    return "hello, world";
}

// Build a shared object with this exact name
BOOST_PYTHON_MODULE(hello_ext)
{
    using namespace boost::python;
    def("greet", greet);
}