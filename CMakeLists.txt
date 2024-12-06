cmake_minimum_required(VERSION 3.15)

project(ccsds VERSION 0.0.1)

#find_package(<> REQUIRED)

find_package(fmt REQUIRED)

add_executable(ccsds src/main.cpp)

target_link_libraries(ccsds fmt::fmt)

install(TARGETS ccsds DESTINATION bin)

set(CMAKE_BUILD_TYPE Debug)