# CHIP-8 Emulator

A simple GUI Chip-8 Emulator used to learn the basics of emulation.

## Local Development

For this solo development, I decided to step into utilizing vcpkg and cmake to help project builds. Currently the CMake file only supports the x64-mingw triplet environment. Further build environments may be added depending on the final scope of the project.

### Windows Environment

For the windows environment, 

To set up CMake (build system automation):

- Download and install the latest stable release of CMake [here](https://cmake.org/download/).
- Place the bin path of CMake in the file `config.txt`.

To set up Ninja (build manager):

- Download and install the latest stable release of Chocolatey [here](https://chocolatey.org/).
- Using Chocolatey, run a command terminal with admin privileges and run ``

To set up vcpkg (package manager):

- Run `git clone https://github.com/Microsoft/vcpkg.git` on the parent folder.
- Run `./vcpkg/bootstrap-vcpkg.bat` to compile vcpkg.
- To run vcpkg commands, use `./vcpkg/vcpkg.exe` on the parent folder.

Always make sure to run the batch file within `/script` from the root folder, or else files will not be found or built in proper directories.
The config file allows future command paths to be included in the project without dependency to the PATH environment variable.ninja
