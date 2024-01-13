echo off
rmdir /s /q build
cmake -B build -S . -DVCPKG_TARGET_TRIPLET="x64-mingw-dynamic" -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake" --preset=default
ninja -C build