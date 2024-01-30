# Idea from https://stackoverflow.com/questions/55963115/how-do-i-make-my-cmake-targets-for-catch2-testing-and-project-running-more-exten
project(main)

set(SHARED_INCLUDES "${CMAKE_CURRENT_LIST_DIR}/include")

add_subdirectory(display)
# add_subdirectory(sound)
add_subdirectory(keyboard)
add_subdirectory(chip8)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE lib::Display)
target_link_libraries(${PROJECT_NAME} PRIVATE lib::Chip8)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${SHARED_INCLUDES}
)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${VCPKG_X64_MINGW}/bin/SDL2.dll" "${PROJECT_BINARY_DIR}"
)
add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${VCPKG_X64_MINGW}/debug/bin/SDL2d.dll" "${PROJECT_BINARY_DIR}"
)