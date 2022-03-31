cmake_minimum_required(VERSION 2.8)
project(Newton C CXX)

if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    print_board_list()
    print_programmer_list()
    include_directories(
        third-party
        src
    )
    add_subdirectory(third-party)
else()
    include_directories(
        src
        tests
    )
    add_subdirectory(tests)
endif()
add_subdirectory(src)