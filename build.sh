#!/bin/sh
cd build && \
cmake --debug-output .. && \
make VERBOSE=1 && \
ctest -VV
