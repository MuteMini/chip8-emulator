cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake" --preset=default
cmake --build build
./build/chip8-interpreter