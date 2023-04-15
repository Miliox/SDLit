# SDLit

Lite SDL resource wrapper to implement RAII idiom.

## Features

* Init with Quit atexit callbacks
* Custom deleter for SDL types
* Wrapper into std::unique\_ptr

## Requirements

* C++20
* G++/Clang Compiler
* Ninja
* Meson
* SDL2, SDL2 Image, SDL2 Mixer, SDL2 TTF

### Mac OS
```bash
brew install meson ninja sdl2 sdl2_image sdl2_mixer sdl2_ttf
```

### Linux

```bash
apt update
apt install meson ninja-build libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

## Build

```bash
# setup
meson setup --wipe builddir

# build
meson compile -C builddir

# optional: enable building examples
meson setup --reconfigure -Dbuild_example=enabled
meson compile -C builddir

```

## Usage

Example:

Creating a window with the [SMPTE color bars](https://en.wikipedia.org/wiki/SMPTE_color_bars):
* See [example/smpte\_test.cpp](example/smpte_test.cpp)

```cpp
#include "sdlit.hpp"

#include <iostream>

int main() {
    // initialize sdl systems
    sdlit::init(SDL_INIT_VIDEO);

    auto const window = sdlit::make_unique(
        SDL_CreateWindow,
        "Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        0U);
    if (not window) { std::cerr << SDL_GetError() << std::endl; return EXIT_FAILURE; }

    auto const renderer = sdlit::make_unique(
        SDL_CreateRenderer,
        window.get(),
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (not renderer) { std::cerr << SDL_GetError() << std::endl; return EXIT_FAILURE; }

    // ...

    return 0;
}
```

## License

[MIT](LICENSE.md)
