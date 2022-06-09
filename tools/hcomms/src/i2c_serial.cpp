// stdlib
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <map>
// third-party
#include <pigpiod_if.h>
// internal imports
#include "i2c_serial.h"

#define BLOCK_SIZE  (32)

using namespace HostComms;

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

I2cSerial::I2cSerial(unsigned i2c_bus, unsigned i2c_address)
{
    // connect to pigpiod on localhost...
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

I2cSerial::~I2cSerial(void)
{
    if (i2c_close(handle_) != 0)
    {
        std::cout << "Failed to close device" << std::endl;
    }
    pigpio_stop();
}

void I2cSerial::transmit(uint8_t to_transmit)
{
    printf("Transmitting byte <0x%02X>...\n", to_transmit);
    int result = i2c_write_byte(handle_, to_transmit);
    if (result < 0)
    {
        throw std::runtime_error(
            "Failed to transmit byte: " + error_code_to_string(result));
    }
}

void I2cSerial::transmit(const uint8_t* to_transmit, size_t n)
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

uint8_t I2cSerial::receive(void)
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
void I2cSerial::receive(uint8_t* result, size_t n)
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

size_t I2cSerial::available(void)
{
    // method not supported with pigpio library
    throw std::runtime_error("Can't do this on I2C");
}
