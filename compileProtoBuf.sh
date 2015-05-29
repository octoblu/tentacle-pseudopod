#!/bin/sh
cd protocols
rm *.pb*
protoc microblu.proto -omicroblu.pb
python ~/Projects/Octoblu/nanopb/generator/nanopb_generator.py microblu.pb
