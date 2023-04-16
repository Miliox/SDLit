#include <cmath>
#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDLit.hpp"

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " 'A TEXT MESSAGE' TTF_FILE FONT_SIZE\n";
        return EXIT_FAILURE;
    }
    char const* const text_message  = argv[1];
    char const* const ttf_file_path = argv[2];
    int const ttf_font_size = std::stoi(argv[3], nullptr, 10);

    SDLit::init(SDL_INIT_VIDEO, /*img*/ 0U, /*mix*/ 0U, /*ttf*/ true);

    auto const window = SDLit::make_unique(
        SDL_CreateWindow,
        "Show Text",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480, 0U);
    if (not window) {
        std::cerr << SDL_GetError() << '\n';
        return EXIT_FAILURE;
    }

    auto const renderer = SDLit::make_unique(
        SDL_CreateRenderer,
        window.get(),
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (not renderer) {
        std::cerr << SDL_GetError() << '\n';
        return EXIT_FAILURE;
    }

    float dpi{};
    float hdpi{};
    float vdpi{};
    if (SDL_GetDisplayDPI(SDL_GetWindowDisplayIndex(window.get()), &dpi, &hdpi, &vdpi)) {
        std::cerr << SDL_GetError() << '\n';
        return EXIT_FAILURE;
    }
    
    auto const font = SDLit::make_unique(TTF_OpenFontDPI, ttf_file_path, ttf_font_size, hdpi, vdpi);
    if (not font) { std::cerr << TTF_GetError() << '\n'; return EXIT_FAILURE; }

    SDL_Color bg_color{0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Color fg_color{0x00, 0x00, 0x00, 0xFF};

    auto const text_surface = SDLit::make_unique(TTF_RenderUTF8_LCD_Wrapped, font.get(), text_message, fg_color, bg_color, 640);
    if (not text_surface) { std::cerr << TTF_GetError() << '\n'; return EXIT_FAILURE; }

    auto const text_texture = SDLit::make_unique(SDL_CreateTextureFromSurface, renderer.get(), text_surface.get());
    if (not text_texture) { std::cerr << SDL_GetError() << '\n'; return EXIT_FAILURE; }

    auto const SDL_SetRenderDrawColorEx = [](SDL_Renderer* renderer, SDL_Color const& color) -> int {
        return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    };

    auto const repaint = [&]() {
        if (SDL_SetRenderDrawColorEx(renderer.get(), bg_color)) {
            std::cerr << SDL_GetError() << '\n';
        }

        if (SDL_RenderClear(renderer.get())) {
            std::cerr << SDL_GetError() << '\n';
        }
        
        int window_width{};
        int window_height{};
        SDL_GetWindowSize(window.get(), &window_width, &window_height);

        int text_width{};
        int text_height{};
        if (SDL_QueryTexture(text_texture.get(), nullptr, nullptr, &text_width, &text_height)) {
            std::cerr << SDL_GetError() << '\n';
        }

        SDL_Rect dst{};
        dst.x = (window_width / 2) - (text_width / 2);
        dst.y = (window_height / 2) - (text_height / 2);
        dst.w = text_width;
        dst.h = text_height;

        if (SDL_RenderCopy(renderer.get(), text_texture.get(), nullptr, &dst)) {
            std::cerr << SDL_GetError() << '\n';
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

