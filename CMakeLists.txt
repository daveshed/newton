cmake_minimum_required(VERSION 2.8)
project(Newton C CXX)
set(CMAKE_CXX_STANDARD 11)
set(TEENSY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third-party/cores/teensy4/)
option(UNIT_TESTS "Build unit tests" ON)

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES ${CMAKE_HOST_SYSTEM_PROCESSOR})
    # Host build...
    # Build for unit tests...
    set(CMAKE_BUILD_TYPE debug)  # compile with debug symbols
endif()
add_subdirectory(mocks)
add_subdirectory(src)
# add_subdirectory(tests)
add_subdirectory(third-party)
add_subdirectory(tools)