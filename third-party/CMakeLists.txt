# Build all third-party libraries
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM")
    # Library build for the target...
    add_library(HX711 HX711/src/HX711.cpp)
    target_include_directories(HX711 PRIVATE ${TEENSY_SOURCE_DIR})
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES ${CMAKE_HOST_SYSTEM_PROCESSOR})
    if(UNIT_TESTS)
        # add_subdirectory(cpputest)
    endif()
else()
    message(FATAL_ERROR "Unrecognised system '${CMAKE_SYSTEM_PROCESSOR}'")
endif()
