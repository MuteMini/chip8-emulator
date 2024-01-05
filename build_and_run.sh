rm -r build
mkdir build
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" --preset=default
cmake --build build
./build/chip8-interpreter