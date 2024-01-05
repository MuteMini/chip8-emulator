cmake_minimum_required(VERSION 3.19)

project(chip8-interpreter LANGUAGES CXX)

set(SOURCES
    src/main.cpp
    src/chip8.cpp
    src/chip8.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)


# add_custom_command(TARGET ${PROJECT_NAME} 
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     "${PROJECT_SOURCE_DIR}/libs" $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )
