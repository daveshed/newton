# /bin/bash
set -ueo pipefail

PROJECT_ROOT=$(dirname $(realpath $0))
BUILD_ABSPATH=${PROJECT_ROOT}/build
CUSTOM_INCLUDE_PATHS=/usr/include/python3.9/
TOOLCHAIN_ABSPATH=${PROJECT_ROOT}/arduino-cmake/cmake/ArduinoToolchain.cmake
HARDWARE_PLATFORM_PATH=/usr/share/arduino/hardware/arduino/avr

function clean {
    echo "#################### CLEANING BUILD ARTIFACTS #######################"
    rm -rf ${BUILD_ABSPATH} && mkdir -p ${BUILD_ABSPATH}
}

function build-linux {
    echo "####################### BUILDING FOR LINUX ##########################"
    # build tests and tools...
    # 1. pass python header paths - broken cmake.config for python?
    # 2. skipping unittests speeds up the build
    cd ${BUILD_ABSPATH} && \
        cmake \
            -DCMAKE_CXX_FLAGS=-I$CUSTOM_INCLUDE_PATHS \
            -DUNIT_TESTS=ON ../ \
        && make
    # install python extension module (skips other installs)...
    # https://stackoverflow.com/a/9192877/18890664
    cmake -DCOMPONENT=runtime -P ${BUILD_ABSPATH}/cmake_install.cmake
}

function build-fw {
    echo "########################## BUILDING FW ##############################"
    echo ${TOOLCHAIN_ABSPATH}
    cd ${BUILD_ABSPATH} && \
        HARDWARE_PLATFORM_PATH=${HARDWARE_PLATFORM_PATH} cmake \
            -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_ABSPATH} \
            -DARDUINO_FOUND:BOOL=False \
            ../ && make
}

function run-unit-tests {
    echo "###################### RUNNING UNIT TESTS ###########################"
    # run unit-tests...
    cd ${BUILD_ABSPATH}/tests && ctest
}

function run-hw-tests {
    echo "###################### RUNNING HW TESTS #############################"
    # test python bindings...
    python $PROJECT_ROOT/tools/hcomms/demo.py
}

function upload-integration-test {
    echo "################# UPLOADING INTEGRATION TEST ########################"
    # upload integration test...
    cd ${BUILD_ABSPATH} && make test_integration-upload
}

function upload-release {
    echo "###################### UPLOADING RELEASE ############################"
    cd ${BUILD_ABSPATH} && make newton_release-upload
}

clean
build-linux
run-unit-tests
# clean
# build-fw
# upload-integration-test
# run-hw-tests
