if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    message(STATUS "Skipping test build for this system.")
else()
    find_package(CppUTest REQUIRED)
    add_executable(test_runner
        mocks.cpp
        main.cpp
    )
    target_link_libraries(test_runner
        forcesensor
        CppUTest
        CppUTestExt
    )
    enable_testing()
    add_test(MyTests test_runner -c)
endif()