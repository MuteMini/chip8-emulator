project(Display_Project)

add_library(Display STATIC display.cpp)
add_library(lib::Display ALIAS Display)

target_link_libraries(Display
    PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
target_include_directories(Display
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)