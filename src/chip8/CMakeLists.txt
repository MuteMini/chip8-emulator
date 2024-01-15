project(Chip8_Project)

add_library(${PROJECT_NAME} STATIC chip8.cpp)
add_library(lib::Chip8 ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC lib::Display)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)