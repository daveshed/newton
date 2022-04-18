cmake_minimum_required(VERSION 2.8)
project(Newton C CXX)
set(CMAKE_CXX_STANDARD 11)

if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    # Build for Arduino..
    print_board_list()
    print_programmer_list()
    add_subdirectory(third-party)
else()
    # Build for unit tests...
    set(CMAKE_BUILD_TYPE debug)  # compile with debug symbols
    # Build python extensions...
    add_subdirectory(tools/hcomms)
    # Build serial library...
    add_subdirectory(third-party/CppLinuxSerial)
endif()
add_subdirectory(mocks)
add_subdirectory(src)
add_subdirectory(tests)