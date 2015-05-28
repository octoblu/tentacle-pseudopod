#!/bin/sh
[ ! -d build ] && mkdir build
cd build && \
cmake  .. && \
make VERBOSE=1 && \
ctest -VV
