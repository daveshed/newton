#include <chrono>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

#include <thread>

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
    , buffer_()
    {
        // non-blocking. Manage timeouts here.
        serial_port_.SetTimeout(0);
        serial_port_.Open();

    }
    ~ConcreteSerial()
    {
        serial_port_.Close();
    }
    void transmit(uint8_t to_transmit) override
    {
        printf("Transmitting byte <0x%02X>...\n", to_transmit);
        std::vector<uint8_t> data(to_transmit);
        serial_port_.WriteBinary(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    void transmit(const uint8_t* to_transmit, size_t n) override
    {
        std::cout << "Transmitting binary data..." << std::endl;
        std::vector<uint8_t> data(to_transmit[0], to_transmit[(n - 1)]);
        serial_port_.WriteBinary(data);
    }
    uint8_t receive(void) override
    {
        std::cout << "Receiving byte..." << std::endl;
        auto start = std::chrono::steady_clock::now();
        auto timeout = std::chrono::milliseconds(1000);
        while (!available())
        {
            fill_buffer_();
            auto elapsed = std::chrono::steady_clock::now() - start;
            if (elapsed > timeout)
            {
                throw std::runtime_error("timeout");
            }
        }
        uint8_t result = buffer_.front();
        buffer_.pop_front();
        std::cout << result << std::endl;
        return result;
    }
    // This signature is not quite right. Surely you mean that the pointer is
    // const and the data is not?
    void receive(uint8_t* result, size_t n) override
    {
        std::cout << "Receiving " << n << " bytes..." << std::endl;
        std::vector<uint8_t> received;
        while (received.size() < n)
        {
            received.push_back(receive());
        }
        memcpy(result, received.data(), n);
    }
    size_t available(void) override
    {
        fill_buffer_();
        auto result = buffer_.size();
        return result;
    }
    void register_callback(Newton::SerialDataCallback* callback) override
    {
        (void)callback;
    }
private:
    void fill_buffer_(void)
    {
        std::vector<uint8_t> data;
        serial_port_.ReadBinary(data);
        buffer_.insert(buffer_.end(), data.begin(), data.end());
    }

    CppLinuxSerial::SerialPort serial_port_;
    std::deque<uint8_t> buffer_;
};

char const* greet()
{
    auto serial_handle = ConcreteSerial(
        "/dev/ttyACM0",
        CppLinuxSerial::BaudRate::B_9600);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    auto host = Newton::HostInterface(serial_handle);
    host.get_reading();
    // SerialPort serial_port(
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

void test()
{
    CppLinuxSerial::SerialPort serial_port(
       "/dev/ttyACM0",
       CppLinuxSerial::BaudRate::B_9600,
       CppLinuxSerial::NumDataBits::EIGHT,
       CppLinuxSerial::Parity::NONE,
       CppLinuxSerial::NumStopBits::ONE);
    serial_port.SetTimeout(-1);  // blocking
    serial_port.Open();
    while (true)
    {
        std::vector<uint8_t> data;
        serial_port.ReadBinary(data);
        for (auto el : data)
        {
            std::cout << el;
        }
        std::cout << std::endl;
    }
}

// Build a shared object with this exact name
BOOST_PYTHON_MODULE(hello_ext)
{
    using namespace boost::python;
    def("greet", greet);
    def("test", test);
}
