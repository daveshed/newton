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
    # build tests and linux serial driver...
    set +u && \
        cd ${BUILD_ABSPATH} && \
        cmake -DCMAKE_CXX_FLAGS=-I$CUSTOM_INCLUDE_PATHS ../ && \
        make
    # run tests...
    cd ${BUILD_ABSPATH}/tests && ctest
    # install python extension module...
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
