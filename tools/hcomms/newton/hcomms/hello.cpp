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
#include <pigpio.h>

#include "hostcomms.h"

// implement serial
class ConcreteSerial : public Newton::SerialHandle {
public:
    ConcreteSerial(unsigned i2c_bus, unsigned i2c_address) : buffer_()
    {
        int result = i2cOpen(i2c_bus, i2c_address, 0);
        if (result < 0)
        {
            throw std::runtime_error("Couldn't open device");
        }
        handle_ = result;
    }
    ~ConcreteSerial()
    {
        if (i2cClose(handle_) != 0)
        {
            std::cout << "Failed to close device" << std::endl;
        }
    }
    void transmit(uint8_t to_transmit) override
    {
        printf("Transmitting byte <0x%02X>...\n", to_transmit);
        std::vector<uint8_t> data(to_transmit);
        int result = i2cWriteByte(handle_, to_transmit);
        if (result != 0)
        {
            throw std::runtime_error("Failed to transmit byte");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    void transmit(const uint8_t* to_transmit, size_t n) override
    {
        std::cout << "Transmitting binary data..." << std::endl;
        for (size_t i = 0; i < n; ++i)
        {
            transmit(to_transmit[i]);
        }
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
        return buffer_.size();
    }
    void register_callback(Newton::SerialDataCallback* callback) override
    {
        (void)callback;
    }
private:
    void fill_buffer_(void)
    {
        // keep reading until there is no more data...
        std::vector<uint8_t> data;
        while (true)
        {
            int result = i2cReadByte(handle_);
            if (result == PI_I2C_READ_FAILED)
            {
                break;
            }
            if (result < 0)
            {
                throw std::runtime_error("Read failed");
            }
            buffer_.push_back(result);
        }
    }

    unsigned handle_;
    std::deque<uint8_t> buffer_;
};

char const* greet()
{
    auto serial_handle = ConcreteSerial(1, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    auto host = Newton::HostInterface(serial_handle);
    host.get_reading();
    return "hello, world";
}

#if 0
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
#endif

// Build a shared object with this exact name
BOOST_PYTHON_MODULE(hello_ext)
{
    using namespace boost::python;
    def("greet", greet);
    // def("test", test);
}
