#!/usr/bin/env bash

cd build

# init cmake from CMakeLists in project root
cmake -DCMAKE_BUILD_TYPE=Release ..

# build
cmake --build .

