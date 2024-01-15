# Idea from https://stackoverflow.com/questions/55963115/how-do-i-make-my-cmake-targets-for-catch2-testing-and-project-running-more-exten
project(main)

find_package(SDL2 CONFIG REQUIRED)

add_subdirectory(display)
add_subdirectory(chip8)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE lib::Display)
target_link_libraries(${PROJECT_NAME} PRIVATE lib::Chip8)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

set(VCPKG_X64_MINGW "${PROJECT_BINARY_DIR}/vcpkg_installed/x64-mingw-dynamic")

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${VCPKG_X64_MINGW}/bin/SDL2.dll" "${PROJECT_BINARY_DIR}"
)
add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${VCPKG_X64_MINGW}/debug/bin/SDL2d.dll" "${PROJECT_BINARY_DIR}"
)