#include "SDLit.hpp"

#include "SDL.h"
#include <iostream>

int main(int argc, char** argv) {
    SDLit::init();

    auto const locales = SDLit::make_unique(SDL_GetPreferredLocales);
    if (not locales) {
        std::cerr << SDL_GetError() << '\n';
    }

    SDL_Locale* locale = locales.get();
    while (locale->language != nullptr) {
        std::cout << "language="  << locale->language
                  << ", country=" << locale->country
                  << std::endl; 
        locale+= 1;
    }

    return EXIT_SUCCESS;
}

