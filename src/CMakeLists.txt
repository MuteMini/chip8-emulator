# Idea from https://stackoverflow.com/questions/55963115/how-do-i-make-my-cmake-targets-for-catch2-testing-and-project-running-more-exten
find_package(SDL2 CONFIG REQUIRED)

add_library(src
    chip8.cpp
    chip8.hpp
)

add_executable(main main.cpp)

target_compile_features(main PRIVATE cxx_std_11)

target_link_libraries(main PRIVATE src)
target_link_libraries(main
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

set(VCPKG_X64_MINGW "${PROJECT_BINARY_DIR}/vcpkg_installed/x64-mingw-dynamic")

add_custom_command(TARGET main
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${VCPKG_X64_MINGW}/bin/SDL2.dll" "${PROJECT_BINARY_DIR}/src"
)
add_custom_command(TARGET main
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${VCPKG_X64_MINGW}/debug/bin/SDL2d.dll" "${PROJECT_BINARY_DIR}/src"
)