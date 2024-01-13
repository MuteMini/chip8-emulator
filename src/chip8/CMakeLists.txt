project(Chip8_Project)

add_library(Chip8 STATIC chip8.cpp)
add_library(lib::Chip8 ALIAS Chip8)

target_include_directories(Chip8
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)