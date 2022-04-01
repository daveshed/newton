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
else()
    # Build library for unit testing...
    add_library(newton
        node.cpp
        serialcomms.cpp
    )
endif()
target_compile_options(newton PRIVATE -Werror -Wall -Wextra)
