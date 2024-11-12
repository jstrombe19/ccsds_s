cmake_minimum_required(VERSION 3.15)

project(ccsds VERSION 0.0.1)

#find_package(<> REQUIRED)

add_executable(ccsds src/main.cpp)

install(TARGETS ccsds DESTINATION bin)

set(CMAKE_BUILD_TYPE Debug)