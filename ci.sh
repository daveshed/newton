# /bin/bash
set -ueo pipefail

PROJECT_ROOT=$(dirname $(realpath $0))
BUILD_ABSPATH=${PROJECT_ROOT}/build

function clean {
    rm -rf ${BUILD_ABSPATH} && \
        mkdir -p ${BUILD_ABSPATH}
}

function run-tests {
    cd ${BUILD_ABSPATH} && \
        cmake ../ && \
        make && \
        cd ${BUILD_ABSPATH}/tests && \
        ctest
}

function build-fw {
    cd ${BUILD_ABSPATH} && \
        cmake --toolchain arduino-cmake/cmake/ArduinoToolchain.cmake ../ && \
        make
}

function flash-fw {
    cd ${BUILD_ABSPATH} && make upload
}

clean
run-tests
clean
build-fw
# flash-fw
