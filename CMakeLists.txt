cmake_minimum_required(VERSION 2.8)
project(Newton C CXX)
set(CMAKE_CXX_STANDARD 11)
option(UNIT_TESTS "Build unit tests" ON)

if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    # Build for Arduino..
    print_board_list()
    print_programmer_list()
    if(DEFINED ENV{HARDWARE_PLATFORM_PATH})
        register_hardware_platform($ENV{HARDWARE_PLATFORM_PATH})
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Build for unit tests...
    set(CMAKE_BUILD_TYPE debug)  # compile with debug symbols
else()
    message(FATAL_ERROR "Unrecognised system '${CMAKE_SYSTEM_NAME}'")
endif()
add_subdirectory(mocks)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(third-party)
add_subdirectory(tools)