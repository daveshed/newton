if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    # Build for arduino...
    generate_arduino_firmware(newton
        SRCS main.cpp node.cpp serialcomms.cpp
        LIBS HX711
        BOARD uno
        PORT /dev/ttyACM0
    )
else()
    # Build for library for unit testing...
    add_library(newton
        node.cpp
        serialcomms.cpp
    )
endif()
target_compile_options(newton PRIVATE -Werror -Wall -Wextra)
