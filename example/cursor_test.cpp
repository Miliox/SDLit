#include <cmath>
#include <iostream>
#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDLit.hpp"

#define WARN_IF_FAIL(result) \
    if (result) { std::cerr << #result << ": " << SDL_GetError() << '\n'; }

#define EXIT_IF_NULL(ptr) \
    if (not ptr) { std::cerr << #ptr << ": " << SDL_GetError() << '\n'; return EXIT_FAILURE; }


int main(int argc, char** argv) {
    SDLit::init(SDL_INIT_VIDEO);

    auto const window = SDLit::make_unique(
        SDL_CreateWindow,
        "Cursor Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0U);
    EXIT_IF_NULL(window);

    auto const renderer = SDLit::make_unique(
        SDL_CreateRenderer,
        window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    EXIT_IF_NULL(renderer);

    std::vector<std::unique_ptr<SDL_Cursor, SDLit::SDL_Deleter>> cursors{};
    for (int id{}; id < SDL_NUM_SYSTEM_CURSORS; ++id) {
        cursors.emplace_back(SDLit::make_unique(SDL_CreateSystemCursor, static_cast<SDL_SystemCursor>(id)));
        EXIT_IF_NULL(cursors.back());
    }
    size_t cursor_index{};

    auto const SDL_SetRenderDrawColorEx = [](SDL_Renderer* renderer, SDL_Color const& color) -> int {
        return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    };

    auto const repaint = [&]() {
        SDL_Color bg_color{0xFF, 0xFF, 0xFF, 0xFF};
        WARN_IF_FAIL(SDL_SetRenderDrawColorEx(renderer.get(), bg_color));
        WARN_IF_FAIL(SDL_RenderClear(renderer.get()));
        
        SDL_RenderPresent(renderer.get());
    };
    repaint();

    bool running{true};
    SDL_Event event{};
    do {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
                    repaint();
                }
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    cursor_index = (cursor_index > 0U) ? (cursor_index - 1U) : (cursors.size() - 1U);
                    SDL_SetCursor(cursors.at(cursor_index).get());
                    break;
                case SDLK_RIGHT:
                    cursor_index = (cursor_index < (cursors.size() - 1U)) ? (cursor_index + 1) : 0U;
                    SDL_SetCursor(cursors.at(cursor_index).get());
                    break;
                case SDLK_ESCAPE:
                    running = false;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    cursor_index = (cursor_index > 0U) ? (cursor_index - 1U) : (cursors.size() - 1U);
                    SDL_SetCursor(cursors.at(cursor_index).get());
                    break;
                case SDL_BUTTON_RIGHT:
                    cursor_index = (cursor_index < (cursors.size() - 1U)) ? (cursor_index + 1) : 0U;
                    SDL_SetCursor(cursors.at(cursor_index).get());
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        SDL_Delay(1'000U / 60U);
    } while (running);

    return 0;
}

