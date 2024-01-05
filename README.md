# CHIP-8 Emulator

A simple GUI Chip-8 Emulator used to learn the basics of emulation.

## Local Development

For this solo development, I decided to step into utilizing vcpkg and cmake to help project builds. Currently the CMake file only supports the x64-mingw triplet environment. Further build environments may be added depending on the final scope of the project.

To set up CMake:

- Download and install the latest stable releast of CMake [here](https://cmake.org/download/).

To set up vcpkg:

- Run `git clone https://github.com/Microsoft/vcpkg.git` on the parent folder.
- Run `./vcpkg/bootstrap-vcpkg.bat` to compile vcpkg.
- To run vcpkg commands, use `./vcpkg/vcpkg.exe` on the parent folder.

After all is set up, the script `build_and_run.sh` can be run.
