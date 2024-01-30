project(Keyboard_Project)

add_library(${PROJECT_NAME} STATIC keyboard.cpp)
add_library(lib::Keyboard ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${SHARED_INCLUDES}
)