set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(ARM_TOOLCHAIN_DIR "/home/dmohamad/dev/gcc-arm-11.2-2022.02-aarch64-arm-none-eabi/bin")

# get_filename_component(ARM_TOOLCHAIN_DIR ${BINUTILS_PATH} DIRECTORY)
# Without that flag CMake is not able to pass test compilation check
if (${CMAKE_VERSION} VERSION_EQUAL "3.6.0" OR ${CMAKE_VERSION} VERSION_GREATER "3.6")
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
else()
    set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=nosys.specs")
endif()

set(CMAKE_C_COMPILER ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${ARM_TOOLCHAIN_DIR}/${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}g++)

# teensy specific options
set(TEENSY_OPTIONS "-D__IMXRT1062__ -DARDUINO_TEENSY41 -DTEENSYDUINO=154")
# configurable hardware options
set(HARDWARE_OPTIONS "-DF_CPU=600000000 -DLAYOUT_UNITED_KINGDOM -DUSB_SERIAL")
# for Cortex M7 with single & double precision FPU
set(CPUOPTIONS "-mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb")
# other libraries to link against
set(ADDITIONAL_LIBS "-larm_cortexM7lfsp_math -lm -lstdc++")
# Default compiler flags
set(CMAKE_C_FLAGS "-Wall -g -O2 ${TEENSY_OPTIONS} -MMD -ffunction-sections -fdata-sections ${HARDWARE_OPTIONS} ${CPUOPTIONS} ${ADDITIONAL_LIBS}")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++14 -felide-constructors -fno-exceptions -fpermissive -fno-rtti -Wno-error=narrowing")
set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")
