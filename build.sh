#!/bin/sh
rm *.pb*
protoc protocols/microblu.proto -omicroblu.pb
python ~/Projects/Octoblu/nanopb/generator/nanopb_generator.py microblu.pb

[ ! -d build ] && mkdir build
cd build && \
cmake  -Dtest=ON .. && \
make VERBOSE=1 && \
ctest -VV
