# /bin/bash
set -ueo pipefail

PROJECT_ROOT=$(dirname $(realpath $0))
BUILD_ABSPATH=${PROJECT_ROOT}/build
CUSTOM_INCLUDE_PATHS=/usr/include/python3.10/
TOOLCHAIN_ABSPATH=${PROJECT_ROOT}/arduino-cmake/cmake/ArduinoToolchain.cmake

function clean {
    rm -rf ${BUILD_ABSPATH} && \
        mkdir -p ${BUILD_ABSPATH}
}

function build-linux {
    # run tests...
    set +u
    cd ${BUILD_ABSPATH} && \
        cmake ../ && \
        CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:$CUSTOM_INCLUDE_PATHS" make && \
        cd ${BUILD_ABSPATH}/tests && \
        ctest
    set -u
    # install python extension...
    cd ${BUILD_ABSPATH} && \
        make install
}

function build-fw {
    cd ${BUILD_ABSPATH} && \
        cmake --toolchain ${TOOLCHAIN_ABSPATH} ../  && \
        make
}

function flash-fw {
    cd ${BUILD_ABSPATH} && make upload
}

clean
build-linux
clean
build-fw
# flash-fw
