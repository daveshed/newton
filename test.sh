#!/bin/bash
set -euo pipefail
SOURCES="test/test_force_plate.cpp test/mocks.cpp forcesensor.cpp"
mkdir -p build
g++ \
    -g $SOURCES \
    -o build/test.o \
    -I test
./build/test.o