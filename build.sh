#!/bin/sh
rm -rf protocols
mkdir protocols
wget --directory-prefix=protocols https://raw.githubusercontent.com/octoblu/tentacle-protocol-buffer/master/microblu.proto
protoc protocols/microblu.proto -oprotocols/microblu.pb
python ~/Projects/Octoblu/nanopb/generator/nanopb_generator.py protocols/microblu.pb

[ ! -d build ] && mkdir build
cd build && \
cmake  -Dtest=ON .. && \
make VERBOSE=1 && \
ctest -VV
