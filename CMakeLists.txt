cmake_minimum_required(VERSION 3.19)
project(chip8_interpreter LANGUAGES CXX)

set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic CACHE STRING "")
set(VCPKG_HOST_TRIPLET x64-mingw-dynamic CACHE STRING "")

add_subdirectory(src)
add_subdirectory(test)
