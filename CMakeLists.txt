cmake_minimum_required(VERSION 3.5)
project(dynamic_lifetime)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/include/kea)

link_libraries(/usr/lib)

add_library(dynamic_lifetime SHARED src/main.cpp src/config.cpp src/config.h)
