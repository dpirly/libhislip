cmake_minimum_required(VERSION 2.6)

project(libhislip)

find_package(Threads REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

