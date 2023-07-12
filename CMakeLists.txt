cmake_minimum_required(VERSION 3.23)

project(Hermes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb") 
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(Dependencies.cmake)

enable_testing()

add_subdirectory(src)
add_subdirectory(bench)
add_subdirectory(test)
