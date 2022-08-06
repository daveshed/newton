if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM")
    # Libraries...
    # add_library(sensor_hx711 sensor.cpp)
    # target_link_libraries(sensor_hx711 HX711)
    # target_include_directories(
    #     sensor_hx711 PUBLIC
    #     ${THIRD_PARTY_DIR}/HX711/src
    #     ${CMAKE_SOURCE_DIR}/include
    # )
    # Firmware executables...
    add_executable(test_fw
        # application.cpp
        # assert.cpp
        main.cpp
        # node.cpp
    )
    # target_link_libraries(test_fw sensor_stub i2c)
    target_link_libraries(test_fw PUBLIC i2c teensy)
    target_link_options(test_fw PUBLIC "-T${TEENSY_SOURCE_DIR}/imxrt1062_t41.ld")
    target_include_directories(test_fw PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${THIRD_PARTY_DIR}/teensy4_i2c/src
    )
    # make hex file for flashing...
    add_custom_command(
        TARGET test_fw POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex -R .eeprom $<TARGET_FILE:test_fw> $<TARGET_FILE:test_fw>.hex
    )



    # generate_arduino_firmware(release_fw
    #     SRCS application.cpp
    #          assert.cpp
    #          main.cpp
    #          node.cpp
    #     LIBS sensor_hx711 i2c
    #     BOARD uno
    #     PORT /dev/ttyACM0
    # )
    # include_directories(
    #     ../include
    #     ../mocks
    #     ../third-party/HX711/src
    #     ${ARDUINO_LIBRARIES_PATH}/Wire/src
    # )
else()
    # 1. Build library for unit testing...
    add_library(newton_target
        node.cpp
    )
    target_include_directories(newton_target PUBLIC ../include/)
    # Only compile with warnings when testing. Otherwise we bring in errors
    # from third-party stuff.
    target_compile_options(newton_target PUBLIC -Werror -Wall -Wextra)
endif()
