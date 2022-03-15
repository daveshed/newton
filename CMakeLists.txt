set(CMAKE_TOOLCHAIN_FILE arduino-cmake/cmake/ArduinoToolchain.cmake) # Arduino Toolchain

cmake_minimum_required(VERSION 2.8)
project(Newton C CXX)

print_board_list()
print_programmer_list()

add_subdirectory(third-party)
add_subdirectory(src)
