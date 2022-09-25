cmake_minimum_required(VERSION 2.8)
project(Newton C CXX)
set(CMAKE_CXX_STANDARD 14)
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third-party)
set(TEENSY_SOURCE_DIR ${THIRD_PARTY_DIR}/cores/teensy4/)
set(ARDUINO_LIBRARIES_PATH ${THIRD_PARTY_DIR}/ArduinoCore-avr/libraries)

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES ${CMAKE_HOST_SYSTEM_PROCESSOR})
    # Host build...
    # Build for unit tests...
    set(CMAKE_BUILD_TYPE debug)  # compile with debug symbols
endif()
add_subdirectory(mocks)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(third-party)
add_subdirectory(tools)