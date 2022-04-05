if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    # Integration test build...
    generate_arduino_firmware(test_integration
        SRCS test_integration.cpp
        LIBS newton sensor_stub
        BOARD uno
        PORT /dev/ttyACM0
    )
    include_directories(
        ../include
        ../mocks
        ../third-party/ArduinoSTL/src
    )
else()
    # Test Runner executable...
    set(CMAKE_BUILD_TYPE debug)  # compile with debug symbols
    find_package(CppUTest REQUIRED)
    add_executable(test_runner
        runner.cpp
        test_comms.cpp
        test_node.cpp
    )
    target_link_libraries(test_runner
        mocks
        newton
        CppUTest
        CppUTestExt
    )
    target_compile_options(test_runner PRIVATE -Werror -Wall -Wextra)
    target_include_directories(test_runner PUBLIC ../mocks/)

    # CTest
    enable_testing()
    add_test(MyTests test_runner -c)
endif()