#include "sdlit.hpp"

#include "SDL.h"

#include <cmath>
#include <iostream>

int main() {
    sdlit::init(SDL_INIT_VIDEO);

    auto const window = sdlit::make_unique(
        SDL_CreateWindow,
        "SMPTE",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        0U);
    if (not window) { std::cerr << SDL_GetError() << '\n'; return EXIT_FAILURE; }

    auto const renderer = sdlit::make_unique(
        SDL_CreateRenderer,
        window.get(),
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (not renderer) { std::cerr << SDL_GetError() << '\n'; return EXIT_FAILURE; }
    
    auto const set_color = [](SDL_Renderer* r, SDL_Color const& c) {
        if (SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a)) { std::cerr << SDL_GetError() << '\n'; }
    };

    auto const repaint = [&]() {
        constexpr SDL_Color white_75pct{180, 180, 180, 255};
        constexpr SDL_Color yellow_75pct{180, 180, 16, 255};
        constexpr SDL_Color cyan_75pct{16, 180, 180, 255};
        constexpr SDL_Color green_75pct{16, 180, 16, 255};
        constexpr SDL_Color magenta_75pct{180, 16, 180, 255};
        constexpr SDL_Color red_75pct{180, 16, 16, 255};
        constexpr SDL_Color blue_75pct{16, 16, 180, 255};

        size_t const num_colors{7U};
        SDL_Color colors[num_colors] = {white_75pct, yellow_75pct, cyan_75pct, green_75pct, magenta_75pct, red_75pct, blue_75pct};

        set_color(renderer.get(), SDL_Color{});
        if (SDL_RenderClear(renderer.get())) { std::cerr << SDL_GetError() << '\n'; }

        int width{};
        int height{};
        SDL_GetWindowSize(window.get(), &width, &height);

        SDL_FRect rect{};
        rect.x = 0.0f;
        rect.y = 0.0f;
        rect.w = static_cast<float>(width) / static_cast<float>(num_colors);
        rect.h = static_cast<float>(height);

        for (int i{}; i < num_colors; ++i) {
            rect.x = rect.w * i;
            set_color(renderer.get(), colors[i]);
            if (SDL_RenderFillRectF(renderer.get(), &rect)) { std::cerr << SDL_GetError() << '\n'; }
        }

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
            default:
                break;
            }
        }
        SDL_Delay(1'000U / 60U);
    } while (running);

    return 0;
}

