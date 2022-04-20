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
    # install python extension module...
    cd ${BUILD_ABSPATH} && \
        make install
}

function build-fw {
    cd ${BUILD_ABSPATH} && \
        cmake --toolchain ${TOOLCHAIN_ABSPATH} ../  && \
        make
}

function run-unit-tests {
    # run unit-tests...
    cd ${BUILD_ABSPATH}/tests && ctest
}

function run-hw-tests {
    # test python bindings...
    python $PROJECT_ROOT/tools/hcomms/demo.py
}

function upload-integration-test {
    # upload integration test...
    cd ${BUILD_ABSPATH} && make test_integration-upload
}

function upload-release {
    cd ${BUILD_ABSPATH} && make newton_release-upload
}

clean
# build-linux
# run-unit-tests
# clean
build-fw
upload-integration-test
# run-hw-tests
