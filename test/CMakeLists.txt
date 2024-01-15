project(unit_tests)

find_package(doctest REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} test_main.cpp)

add_compile_definitions(TESTING_ON)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE lib::Chip8)
target_link_libraries(${PROJECT_NAME} PRIVATE doctest::doctest)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${VCPKG_X64_MINGW}/bin/SDL2.dll" "${PROJECT_BINARY_DIR}"
)
add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${VCPKG_X64_MINGW}/debug/bin/SDL2d.dll" "${PROJECT_BINARY_DIR}"
)