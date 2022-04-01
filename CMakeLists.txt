cmake_minimum_required(VERSION 2.8)
project(Newton C CXX)
option(INTEGRATION_TEST "Stub hardware/perpheral interfaces" OFF)

if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    # Build for Arduino..
    print_board_list()
    print_programmer_list()
    include_directories(
        include
        third-party
    )
    add_subdirectory(third-party)
else()
    # Build for unit tests...
    include_directories(
        include
        tests
    )
    add_subdirectory(tests)
endif()
add_subdirectory(src)