if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    # Arduino build for release...
    generate_arduino_library(sensor
        SRCS sensor.cpp
        LIBS HX711
        BOARD uno
    )
    generate_arduino_library(newton
        SRCS node.cpp serialcomms.cpp
        BOARD uno
    )
    generate_arduino_firmware(newton_release
        SRCS main.cpp
        LIBS newton sensor
        BOARD uno
        PORT /dev/ttyACM0
    )
    include_directories(
        ../include
        ../third-party
        ../third-party/ArduinoSTL/src
    )
else()
    # Build library for unit testing...
    add_library(newton
        node.cpp
        serialcomms.cpp
    )
    target_include_directories(newton PUBLIC ../include/)
    # Only compile with warnings when testing. Otherwise we bring in errors
    # from third-party stuff.
    target_compile_options(newton PRIVATE -Werror -Wall -Wextra)
endif()
