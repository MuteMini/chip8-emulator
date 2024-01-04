cmake_minimum_required(VERSION 3.7)

project(chip8-interpreter 
            DESCRIPTION "SDL based Chip8 interpreter"
            LANGUAGES CXX)

set(SOURCES
    src/main.cpp
    src/chip8.cpp
)

# find_package(SDL2 REQUIRED)

# include_directories(${SDL2_INCLUDE_DIR})
# link_libraries(${SDL2_LIBRARY})

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

