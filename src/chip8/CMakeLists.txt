project(Chip8_Project)

add_library(${PROJECT_NAME} STATIC chip8.cpp)
add_library(lib::Chip8 ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${SHARED_INCLUDES}
)