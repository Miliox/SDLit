#include "SDLit.hpp"

#include <cstdlib>
#include <mutex>
#include <utility>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdocumentation"
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#pragma GCC diagnostic pop

namespace SDLit {

void init(
    std::uint32_t sdl_init_flags,
    std::uint32_t img_init_flags,
    std::uint32_t mix_init_flags,
    bool          shall_init_ttf) noexcept {

    static std::once_flag quit_once_flag{};
    std::call_once(quit_once_flag, []() {
        std::atexit(SDL_Quit);
        std::atexit(IMG_Quit);
        std::atexit(Mix_Quit);
        std::atexit(TTF_Quit);
    });

    if (sdl_init_flags && SDL_Init(sdl_init_flags)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "SDL_Init failed: %s", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }

    if (img_init_flags && img_init_flags != IMG_Init(img_init_flags)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "IMG_Init failed: %s", IMG_GetError());
        std::exit(EXIT_FAILURE);
    }

    if (mix_init_flags && mix_init_flags != Mix_Init(mix_init_flags)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "Mix_Init failed: %s", Mix_GetError());
        std::exit(EXIT_FAILURE);
    }

    if (shall_init_ttf && TTF_Init()) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "TTF_Init failed: %s", TTF_GetError());
        std::exit(EXIT_FAILURE);
    }
}

void SDL_Deleter::operator()(_TTF_Font* ptr) noexcept {
    if (ptr != nullptr) {
        TTF_CloseFont(ptr);
    }
}

void SDL_Deleter::operator()(SDL_Cursor* ptr) noexcept {
    if (ptr != nullptr) {
        SDL_FreeCursor(ptr);
    }
}

void SDL_Deleter::operator()(SDL_Renderer* ptr) noexcept {
    if (ptr != nullptr) {
        SDL_DestroyRenderer(ptr);
    }
}

void SDL_Deleter::operator()(SDL_Surface* ptr) noexcept {
    if (ptr != nullptr) {
        SDL_FreeSurface(ptr);
    }
}

void SDL_Deleter::operator()(SDL_Texture* ptr) noexcept {
    if (ptr != nullptr) {
        SDL_DestroyTexture(ptr);
    }
}

void SDL_Deleter::operator()(SDL_Window* ptr) noexcept {
    if (ptr != nullptr) {
        SDL_DestroyWindow(ptr);
    }
}

void SDL_Deleter::operator()(Mix_Chunk* ptr) noexcept {
    if (ptr != nullptr) {
        Mix_FreeChunk(ptr);
    }
}

void SDL_Deleter::operator()(Mix_Music* ptr) noexcept {
    if (ptr != nullptr) {
        Mix_FreeMusic(ptr);
    }
}


}
