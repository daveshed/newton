set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(ARM_TOOLCHAIN_DIR "/home/dmohamad/dev/gcc-arm-11.2-2022.02-aarch64-arm-none-eabi/bin")

# Without that flag CMake is not able to pass test compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${ARM_TOOLCHAIN_DIR}/${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}g++)

# teensy specific options
set(TEENSY_OPTIONS "-D__IMXRT1062__ -DARDUINO_TEENSY41 -DTEENSYDUINO=154")
# configurable hardware options
set(HARDWARE_OPTIONS "-DF_CPU=600000000 -DLAYOUT_UNITED_KINGDOM -DUSB_SERIAL")
# for Cortex M7 with single & double precision FPU
set(CPUOPTIONS "-mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb")
# other libraries to link against eg. arm libs available here...
# https://github.com/ARM-software/CMSIS/tree/master/CMSIS/Lib/ARM
# arm_cortex...fsp_math should be copied to the compiler lib dir here...
# gcc-arm-<verision>-<arch>-arm-none-eabi/arm-non-eabi/lib/libarm_cortexM7lfsp_math.a
set(ADDITIONAL_LIBS "-larm_cortexM7lfsp_math -lm -lstdc++")
# Default compiler flags...
set(CMAKE_C_FLAGS "-Wall -g -O2 ${TEENSY_OPTIONS} -MMD -ffunction-sections -fdata-sections ${HARDWARE_OPTIONS} ${CPUOPTIONS} ${ADDITIONAL_LIBS}")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++14 -felide-constructors -fno-exceptions -fpermissive -fno-rtti -Wno-error=narrowing")
set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")
set(CMAKE_EXE_LINKER_FLAGS "-Os -Wl,--gc-sections,--relax ${CPUOPTIONS}")
