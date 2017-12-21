cmake_minimum_required(VERSION 2.6)

project(libhislip)

# echo compile commadline
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(Threads REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

