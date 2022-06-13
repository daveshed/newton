if(${CMAKE_SYSTEM_NAME} MATCHES "ARM")
    # Libraries...
    generate_arduino_library(i2c
        SRCS ${ARDUINO_LIBRARIES_PATH}/Wire/src/Wire.cpp
             ${ARDUINO_LIBRARIES_PATH}/Wire/src/utility/twi.c
        BOARD uno
    )
    generate_arduino_library(sensor_hx711
        SRCS sensor.cpp
        LIBS HX711
        BOARD uno
    )
    # Firmware executables...
    generate_arduino_firmware(test_fw
        SRCS application.cpp
             assert.cpp
             main.cpp
             node.cpp
        LIBS sensor_stub i2c
        BOARD uno
        PORT /dev/ttyACM0
    )
    generate_arduino_firmware(release_fw
        SRCS application.cpp
             assert.cpp
             main.cpp
             node.cpp
        LIBS sensor_hx711 i2c
        BOARD uno
        PORT /dev/ttyACM0
    )
    include_directories(
        ../include
        ../mocks
        ../third-party/HX711/src
        ${ARDUINO_LIBRARIES_PATH}/Wire/src
    )
else()
    # 1. Build library for unit testing...
    add_library(newton_target
        node.cpp
    )
    target_include_directories(newton_target PUBLIC ../include/)
    # Only compile with warnings when testing. Otherwise we bring in errors
    # from third-party stuff.
    target_compile_options(newton_target PUBLIC -Werror -Wall -Wextra)
endif()
