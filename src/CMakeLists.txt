if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    generate_arduino_firmware(newton
        SRCS newton.cpp forcesensor.cpp serialcomms.cpp
        LIBS HX711
        BOARD uno
        PORT /dev/ttyACM0
    )
else()
    add_library(newton
        forcesensor.cpp
        serialcomms.cpp
    )
endif()
target_compile_options(newton PRIVATE -Werror -Wall -Wextra)
