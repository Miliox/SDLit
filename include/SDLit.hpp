#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>

struct Mix_Chunk;
struct _Mix_Music;
struct _TTF_Font;

struct SDL_AudioSpec;
struct SDL_Cursor;
struct SDL_Locale;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;

namespace SDLit {

/// Init SDL and register at exit callbacks to tear SDL down
void init(
    std::uint32_t sdl_init_flags = 0,
    std::uint32_t img_init_flags = 0,
    std::uint32_t mix_init_flags = 0,
    bool          shall_init_ttf = false) noexcept;

/// SDL types custom deleter
class SDL_Deleter final {
public:
    void operator()(Mix_Chunk* ptr)    noexcept;
    void operator()(_Mix_Music* ptr)   noexcept;
    void operator()(_TTF_Font* ptr)    noexcept;
    void operator()(SDL_Cursor* ptr)   noexcept;
    void operator()(SDL_Locale* ptr)   noexcept;
    void operator()(SDL_Renderer* ptr) noexcept;
    void operator()(SDL_Surface* ptr)  noexcept;
    void operator()(SDL_Texture* ptr)  noexcept;
    void operator()(SDL_Window* ptr)   noexcept;
};

/// Simple Helper to wrap return of SDL_Create calls into unique_ptr
auto make_unique(auto&& fun, auto&&... args) {
    auto* const ptr = fun(std::forward<decltype(args)>(args)...);
    return std::unique_ptr<typename std::remove_pointer<decltype(ptr)>::type, SDL_Deleter>{ptr};
}

}
