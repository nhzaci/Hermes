#!/usr/bin/env bash

mkdir -p && cd build

# init cmake from CMakeLists in project root
cmake -DCMAKE_BUILD_TYPE=Release ..

# build
cmake --build .

