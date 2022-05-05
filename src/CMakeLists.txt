if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
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
             buffer.cpp
             main.cpp
             node.cpp
             serial.cpp
        LIBS sensor_stub i2c
        BOARD uno
        PORT /dev/ttyACM0
    )
    generate_arduino_firmware(release_fw
        SRCS application.cpp
             buffer.cpp
             main.cpp
             node.cpp
             serial.cpp
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
        buffer.cpp
        node.cpp
    )
    target_include_directories(newton_target PUBLIC
        ${CMAKE_SOURCE_DIR}/include/)
    # Only compile with warnings when testing. Otherwise we bring in errors
    # from third-party stuff.
    target_compile_options(newton_target PRIVATE -Werror -Wall -Wextra)
    # 2. Build library for python-bindings...
    add_library(newton_host
        hostcomms.cpp
    )
    target_include_directories(newton_host PUBLIC
        ${CMAKE_SOURCE_DIR}/include/)
    set_property(TARGET newton_host PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_compile_options(newton_host PRIVATE -Werror -Wall -Wextra)
endif()
