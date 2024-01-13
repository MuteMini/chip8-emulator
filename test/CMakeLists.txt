find_package(doctest REQUIRED)

add_executable(unit_tests test_main.cpp)

add_compile_definitions(TESTING_ON)

target_compile_features(unit_tests PRIVATE cxx_std_17)

target_link_libraries(unit_tests PRIVATE lib::Chip8)
target_link_libraries(unit_tests PRIVATE doctest::doctest)