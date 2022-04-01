if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    # Stubbed hardware for Arduino integration tests...
    generate_arduino_library(sensor_stub
        SRCS sensor.cpp
        BOARD uno
    )
else()
    add_library(mocks
        mocks.cpp
        sensor.cpp
    )
endif()