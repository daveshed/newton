cmake_minimum_required(VERSION 2.8)
project(Newton C CXX)
set (CMAKE_CXX_STANDARD 11)

if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    # Build for Arduino..
    print_board_list()
    print_programmer_list()
    include_directories(
        mocks
        include
        third-party
    )
    add_subdirectory(third-party)
else()
    # Build for unit tests...
    include_directories(
        include
        mocks
    )
endif()
add_subdirectory(mocks)
add_subdirectory(src)
add_subdirectory(tests)