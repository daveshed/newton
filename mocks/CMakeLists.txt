if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM")
    # Stubbed hardware for integration tests...
    add_library(sensor_stub STATIC sensor.cpp)
    target_include_directories(sensor_stub PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${TEENSY_SOURCE_DIR}
    )
else()
    add_library(mocks
        mocks.cpp
        sensor.cpp
    )
    target_include_directories(
        mocks PUBLIC
        ${CMAKE_SOURCE_DIR}/include/
        ${TEENSY_SOURCE_DIR}
    )
endif()