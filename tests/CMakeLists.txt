# Unit test build only
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND UNIT_TESTS)
    # Test Runner executable...
    add_executable(test_runner
        runner.cpp
        test_buffer.cpp
        test_node.cpp
    )
    target_link_libraries(test_runner
        mocks
        newton_host
        newton_target
        CppUTest
        CppUTestExt
    )
    target_compile_options(test_runner PRIVATE -Werror -Wall -Wextra)
    target_include_directories(test_runner PRIVATE ../mocks/)

    # CTest
    enable_testing()
    add_test(MyTests test_runner -c)
endif()