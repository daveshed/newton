# Build our pyhon->c++ bindings for serial comms on the host
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES ${CMAKE_HOST_SYSTEM_PROCESSOR})
    find_library(PIGPIO NAMES pigpiod_if)
    if(${PIGPIO} MATCHES "NOTFOUND")
        # Skip this build so we can still compile on Linux for unit testing only
        message(STATUS "Skipping python build: pigpio library is not available")
    else()
        find_library(PYTHON3 NAMES python3 python3.9)
        find_library(BOOST_PYTHON3 NAMES boost_python3 boost_python39)
        add_library(hcomms_ext SHARED
            src/bindings.cpp
            src/hcomms.cpp
            src/i2c_serial.cpp
        )
        set_property(TARGET hcomms_ext PROPERTY POSITION_INDEPENDENT_CODE ON)
        # We must ensure that the library is named 'hello_ext'. If it's prefixe
        # with the default 'lib', then python won't import it.
        set(CMAKE_SHARED_LIBRARY_PREFIX "")
        target_include_directories(hcomms_ext PRIVATE
            include
            ${CMAKE_SOURCE_DIR}/include
        )
        target_link_libraries(hcomms_ext
            ${BOOST_PYTHON3}
            ${PIGPIO}
            ${PYTHON3}
        )
        target_compile_options(hcomms_ext PRIVATE -Werror -Wall -Wextra)
        # install the shared library into the python package for distribution...
        install(TARGETS hcomms_ext
            DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/newton/hcomms/
            COMPONENT runtime
        )
    endif()
endif()