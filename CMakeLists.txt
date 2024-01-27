cmake_minimum_required(VERSION 3.19)
project(chip8_interpreter LANGUAGES CXX)

find_package(doctest CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

set(VCPKG_X64_MINGW "${PROJECT_BINARY_DIR}/vcpkg_installed/x64-mingw-dynamic")

add_subdirectory(src)
add_subdirectory(test)
