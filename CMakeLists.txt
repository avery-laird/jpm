cmake_minimum_required(VERSION 3.8)
project(jekyllposts)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES jpm.c)
add_executable(jekyllposts ${SOURCE_FILES})