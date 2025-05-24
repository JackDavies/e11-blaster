#! /bin/bash

mkdir -p build
cd build
cmake -DPICO_SDK_PATH=../../../pico-sdk ..
make output
cd ..
