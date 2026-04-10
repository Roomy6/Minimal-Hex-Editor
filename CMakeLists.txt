
cmake_minimum_required(VERSION 3.10)

project(mbe C)

add_executable(mbe mbe.c)

target_compile_options(mbe PRIVATE -Oz)
target_link_options(mbe PRIVATE -s -Wl,--gc-sections)
