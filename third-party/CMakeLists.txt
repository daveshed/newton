# Build all third-party libraries
if(${CMAKE_SYSTEM_NAME} MATCHES "Arduino")
    # Target build
    generate_arduino_library(HX711
        SRCS HX711.cpp
        HDRS HX711.h
        BOARD uno)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_subdirectory(cpputest)
    # Skip loads of cpputest test builds
    set(TESTS OFF CACHE BOOL "Enable tests" FORCE)
else()
    message(FATAL_ERROR "Unrecognised system '${CMAKE_SYSTEM_NAME}'")
endif()
