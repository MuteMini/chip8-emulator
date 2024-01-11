find_package(doctest REQUIRED)

add_executable (unit_tests test_main.cpp)

target_compile_features(unit_tests PRIVATE cxx_std_11)

target_link_libraries(unit_tests PRIVATE src)
target_link_libraries(unit_tests PRIVATE doctest::doctest)