# /bin/bash
set -ueo pipefail

PROJECT_ROOT=$(dirname $(realpath $0))
BUILD_ABSPATH=${PROJECT_ROOT}/build
THIRD_PARTY_ABSPATH=${PROJECT_ROOT}/third-party
CUSTOM_INCLUDE_PATHS=/usr/include/python3.9/
TOOLCHAIN_ABSPATH=${PROJECT_ROOT}/arm-toolchain.cmake

function clean() {
    echo "#################### CLEANING BUILD ARTIFACTS #######################"
    rm -rf ${BUILD_ABSPATH} && mkdir -p ${BUILD_ABSPATH}
}

function build-linux() {
    echo "####################### BUILDING FOR LINUX ##########################"
    # build tests and tools...
    # 1. pass python header paths - broken cmake.config for python?
    # 2. skipping unittests speeds up the build
    cd ${BUILD_ABSPATH} && \
        cmake -DCMAKE_CXX_FLAGS=-I$CUSTOM_INCLUDE_PATHS ../ \
        && make
    # install python extension module (skips other installs)...
    # https://stackoverflow.com/a/9192877/18890664
    cmake -DCOMPONENT=runtime -P ${BUILD_ABSPATH}/cmake_install.cmake
    cd ${THIRD_PARTY_ABSPATH}/teensy_loader_cli && make
}

function build-fw() {
    echo "########################## BUILDING FW ##############################"
    echo ${TOOLCHAIN_ABSPATH}
    cd ${BUILD_ABSPATH} && \
            cmake -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_ABSPATH} \
            ../ && make
}

function run-unit-tests() {
    echo "###################### RUNNING UNIT TESTS ###########################"
    # run unit-tests...
    cd ${BUILD_ABSPATH}/tests && ctest
}

function run-hw-tests() {
    echo "###################### RUNNING HW TESTS #############################"
    # test python bindings...
    python $PROJECT_ROOT/tools/hcomms/demo.py
}

function flash-fw-binary() {
    FW_BINARY_ABSPATH=$1
    echo "################# UPLOADING '$FW_BINARY_ABSPATH' ########################"
    # upload integration test...
    cd ${THIRD_PARTY_ABSPATH}/teensy_loader_cli && \
        ./teensy_loader_cli -swv --mcu=TEENSY41 ${BUILD_ABSPATH}/src/test_fw.hex
}

# clean
# build-linux
# run-unit-tests
# clean
build-fw
flash-fw-binary "${BUILD_ABSPATH}/src/test_fw.hex"
# run-hw-tests
# flash-fw-binary "${BUILD_ABSPATH}/src/release_fw.hex"
