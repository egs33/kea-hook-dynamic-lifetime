cmake_minimum_required(VERSION 3.5)
project(dynamic_lifetime)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/include/kea)

link_libraries(/usr/lib)

add_library(dynamic_lifetime SHARED src/main.cpp)

target_link_libraries(dynamic_lifetime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/go_dist/libdynamic_lifetime_go.a)
