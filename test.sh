#!/bin/bash
set -euo pipefail
mkdir -p build
g++ -g test/test.cpp test/mocks.cpp forcesensor.cpp -o build/test.o -I test
./build/test.o