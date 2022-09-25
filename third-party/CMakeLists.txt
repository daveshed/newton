# Build all third-party libraries
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM")
    # Libraries built for the target...
    ### TEENSY SOURCES ###
    FILE(GLOB TEENSY_SOURCES ${TEENSY_SOURCE_DIR}/*.c ${TEENSY_SOURCE_DIR}/*.cpp)
    add_library(teensy STATIC ${TEENSY_SOURCES})
    target_include_directories(teensy PUBLIC ${TEENSY_SOURCE_DIR})
    ### LOAD CELL SOURCES ###
    add_library(HX711 STATIC HX711/src/HX711.cpp)
    target_include_directories(HX711 PUBLIC ${TEENSY_SOURCE_DIR})
    ### I2C SOURCES ###
    add_library(i2c STATIC
        teensy4_i2c/src/i2c_driver_wire.cpp
        teensy4_i2c/src/i2c_register_slave.cpp
        teensy4_i2c/src/imx_rt1060/imx_rt1060_i2c_driver.cpp
    )
    target_include_directories(i2c PUBLIC ${TEENSY_SOURCE_DIR})
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES ${CMAKE_HOST_SYSTEM_PROCESSOR})
    add_subdirectory(cpputest)
else()
    message(FATAL_ERROR "Unrecognised system '${CMAKE_SYSTEM_PROCESSOR}'")
endif()
