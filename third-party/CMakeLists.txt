# Build all third-party libraries
if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    # Library build for the target...
    generate_arduino_library(HX711
        SRCS HX711/src/HX711.cpp
        HDRS HX711/src/HX711.h
        BOARD uno
    )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(UNIT_TESTS)
        add_subdirectory(cpputest)
    endif()
else()
    message(FATAL_ERROR "Unrecognised system '${CMAKE_SYSTEM_NAME}'")
endif()
