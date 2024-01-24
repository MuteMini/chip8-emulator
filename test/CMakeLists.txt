project(unit_tests)

find_package(doctest REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} test.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE lib::Chip8)
target_link_libraries(${PROJECT_NAME} PRIVATE doctest::doctest)