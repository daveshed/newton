if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM")
    ## LIBRARIES ###
    add_library(sensor_hx711 sensor.cpp)
    target_link_libraries(sensor_hx711 HX711)
    target_include_directories(
        sensor_hx711 PUBLIC
        ${THIRD_PARTY_DIR}/HX711/src
        ${CMAKE_SOURCE_DIR}/include
    )

    ### FW BINARIES ###
    set(FW_SOURCES "application.cpp" "assert.cpp" "main.cpp" "node.cpp")
    # TEST FW...
    add_executable(test_fw ${FW_SOURCES})
    target_link_libraries(test_fw PUBLIC
        i2c
        sensor_stub
        teensy
    )
    target_link_options(test_fw PUBLIC "-T${TEENSY_SOURCE_DIR}/imxrt1062_t41.ld")
    target_include_directories(test_fw PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${THIRD_PARTY_DIR}/teensy4_i2c/src
    )
    # make hex file for flashing...
    add_custom_command(
        TARGET test_fw POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex -R .eeprom
        $<TARGET_FILE:test_fw> $<TARGET_FILE:test_fw>.hex
    )
    # RELEASE FW...
    add_executable(release_fw ${FW_SOURCES})
    target_link_libraries(release_fw PUBLIC
        i2c
        sensor_hx711
        teensy
    )
    target_link_options(release_fw PUBLIC "-T${TEENSY_SOURCE_DIR}/imxrt1062_t41.ld")
    target_include_directories(release_fw PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${THIRD_PARTY_DIR}/teensy4_i2c/src
    )
    # make hex file for flashing...
    add_custom_command(
        TARGET release_fw POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex -R .eeprom
        $<TARGET_FILE:release_fw> $<TARGET_FILE:release_fw>.hex
    )
else()
    # 1. Build library for unit testing...
    add_library(newton_target
        node.cpp
    )
    target_include_directories(newton_target PUBLIC ../include/)
    # Only compile with warnings when testing. Otherwise we bring in errors from third-party stuff.
    target_compile_options(newton_target PUBLIC -Werror -Wall -Wextra)
endif()
