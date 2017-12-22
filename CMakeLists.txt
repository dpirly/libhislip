cmake_minimum_required(VERSION 2.6)

project(libhislip)

add_compile_options(-Wall)
add_compile_options(-g)

find_package(Threads REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

