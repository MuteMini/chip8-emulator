# CHIP-8 Emulator

A simple GUI Chip-8 Emulator used to learn the basics of emulation.

## Local Development

The makefile assumes that within the main directory there exists the following:

```
SDL2.dll
\libs
    \include
        ...
    \lib
        ...
```

The reasoning for this type of installation leans on prioritizing the modularity of development environments.
The `SDL2.dll`, `\include` and `\lib` files/folders can be downloaded from the SDL website [here.](https://github.com/libsdl-org/SDL/releases/tag/release-2.28.5)
