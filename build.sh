#!/usr/bin/env bash

# by default, type is release
TYPE=Debug;

# debug / release through -d or -r flag
while getopts rd flag
do
    case $flag in
        r) TYPE=Release;;
        d) TYPE=Debug;;
    esac
done

echo
echo "///////////////////////////"
echo "Building in \"$TYPE\" mode"
echo "///////////////////////////"
echo

sleep 0.5

# install deps with conan
conan install . --output-folder=build --build=missing -s build_type=$TYPE

# cd into build and build for release
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=$TYPE
cmake --build .

# change back to current dir
cd ..