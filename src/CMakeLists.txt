if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    generate_arduino_library(forcesensor
        SRCS forcesensor.cpp
        HDRS forcesensor.h
        BOARD uno
    )

    generate_arduino_firmware(newton
        SRCS newton.cpp
        LIBS HX711 forcesensor
        BOARD uno
        PORT /dev/ttyACM0
    )
else()
    add_library(forcesensor
        forcesensor.cpp
    )
endif()