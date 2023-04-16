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

* Include __SDL__ headers
* Include __SDLit.hpp__
* Init SDL subsystems using __SDLit::init()__
* Invoke SDL function that allocate using __SDLit::make_unique()__ for RAII

```cpp
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDLit.hpp"

int main() {
    // initialize sdl systems
    SDLit::init(SDL_INIT_EVERYTHING, IMG_INIT_JPG | IMG_INIT_PNG, MIX_INIT_MP3, true);

    auto const window = SDLit::make_unique(
        SDL_CreateWindow, "Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0U);
    if (not window) { /* omitted */ }

    auto const renderer = SDLit::make_unique(
        SDL_CreateRenderer, window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (not renderer) { /* omitted */ }

    // omitted

    return 0;
}
```

For more details check [examples](examples) folder.

## License

[MIT](LICENSE.md)
