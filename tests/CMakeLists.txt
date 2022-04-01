if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    message(STATUS "Skipping test build for this system.")
else()
    # Test Runner executable
    set(CMAKE_BUILD_TYPE debug)  # compile with debug symbols
    find_package(CppUTest REQUIRED)
    add_executable(test_runner
        mocks.cpp
        runner.cpp
        test_comms.cpp
        test_node.cpp
    )
    target_link_libraries(test_runner
        newton
        CppUTest
        CppUTestExt
    )
    target_compile_options(test_runner PRIVATE -Werror -Wall -Wextra)

    # CTest
    enable_testing()
    add_test(MyTests test_runner -c)
endif()