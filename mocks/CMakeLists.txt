if(${CMAKE_SYSTEM_NAME} MATCHES "ARM")
    # Stubbed hardware for Arduino integration tests...
    generate_arduino_library(sensor_stub
        SRCS sensor.cpp
        BOARD uno
    ) # does not work with target_include_directories
    include_directories(
        ../include
    )
else()
    add_library(mocks
        mocks.cpp
        sensor.cpp
    )
    target_include_directories(mocks PUBLIC ../include/)
endif()