#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <pigpiod_if.h>

#include "hostcomms.h"

#define BLOCK_SIZE  (32)


std::string error_code_to_string(int code)
{
    std::map<int, std::string> error_codes {
        {PI_BAD_I2C_BUS, "PI_BAD_I2C_BUS"},
        {PI_BAD_I2C_ADDR, "PI_BAD_I2C_ADDR"},
        {PI_BAD_FLAGS, "PI_BAD_FLAGS"},
        {PI_NO_HANDLE, "PI_NO_HANDLE"},
        {PI_I2C_READ_FAILED, "PI_I2C_READ_FAILED"},
        {PI_I2C_WRITE_FAILED, "PI_I2C_WRITE_FAILED"},
        {PI_I2C_OPEN_FAILED, "PI_I2C_OPEN_FAILED"},
        {PI_NOT_INITIALISED, "PI_NOT_INITIALISED"},
        {PI_INIT_FAILED, "PI_INIT_FAILED"},
    };
    try {
        return error_codes.at(code);
    } catch (std::out_of_range const&) {
        return "UNKNOWN (" + std::to_string(code) + ")";
    }
}

// implement serial
class ConcreteSerial : public Newton::SerialHandle {
public:
    ConcreteSerial(unsigned i2c_bus, unsigned i2c_address)
    {
        // connect on localhost...
        int gpio_status = pigpio_start(NULL, NULL);
        if (gpio_status < 0)
        {
            throw std::runtime_error(
                "Failed to initialise gpio: "
                + error_code_to_string(gpio_status));
        }
        int i2c_status = i2c_open(i2c_bus, i2c_address, 0);
        if (i2c_status < 0)
        {
            throw std::runtime_error(
                "Couldn't open device: " + error_code_to_string(i2c_status));
        }
        handle_ = i2c_status;
    }
    ~ConcreteSerial()
    {
        if (i2c_close(handle_) != 0)
        {
            std::cout << "Failed to close device" << std::endl;
        }
        pigpio_stop();
    }
    void transmit(uint8_t to_transmit) override
    {
        printf("Transmitting byte <0x%02X>...\n", to_transmit);
        int result = i2c_write_byte(handle_, to_transmit);
        if (result < 0)
        {
            throw std::runtime_error(
                "Failed to transmit byte: " + error_code_to_string(result));
        }
    }
    void transmit(const uint8_t* to_transmit, size_t n) override
    {
        std::cout << "Transmitting binary data..." << std::endl;
        if (n > BLOCK_SIZE)
        {
            throw std::runtime_error("Exceeded block size");
        }
        int result = i2c_write_device(handle_, (char*)to_transmit, n);
        if (result < 0)
        {
            throw std::runtime_error("Write failed");
        }
    }
    uint8_t receive(void) override
    {
        std::cout << "Receiving byte..." << std::endl;
        int result = i2c_read_byte(handle_);
        if (result < 0)
        {
            throw std::runtime_error(
                "Read failed: " + error_code_to_string(result));
        }
        std::cout << "Got byte '" << result << "'" << std::endl;
        return result;
    }
    // This signature is not quite right. Surely you mean that the pointer is
    // const and the data is not?
    void receive(uint8_t* result, size_t n) override
    {
        std::cout << "Receiving " << n << " bytes..." << std::endl;
        if (n > BLOCK_SIZE)
        {
            throw std::runtime_error("Exceeded read block size");
        }
        int status = i2c_read_device(handle_, (char*)result, n);
        if (status < 0)
        {
            throw std::runtime_error(
                "Read failed: " + error_code_to_string(status));
        }
        std::cout << "Here's your data... ";
        for (size_t i = 0; i < n; ++i)
        {
            printf("0x%02x ", result[i]);
        }
        std::cout << std::endl;
    }
    size_t available(void) override
    {
        // method not supported with pigpio library
        throw std::runtime_error("Can't do this on I2C");
    }
    void register_callback(Newton::SerialDataCallback* callback) override
    {
        (void)callback;
    }
private:
    unsigned handle_;
};

char const* greet()
{
    auto serial_handle = ConcreteSerial(1, 0x11);
    auto host = Newton::HostInterface(serial_handle);
    Newton::Measurement_t measurement = host.get_reading();
    std::cout << measurement.raw_data << std::endl;
    return "hello, world";
}

// Build a shared object with this exact name
BOOST_PYTHON_MODULE(hello_ext)
{
    using namespace boost::python;
    def("greet", greet);
}
