#!/bin/bash
set -euo pipefail
SOURCES="test/test_comms.cpp test/mocks.cpp serialcomms.cpp forcesensor.cpp"
mkdir -p build
g++ \
    -g $SOURCES \
    -o build/test.o \
    -I test
./build/test.o