#include "sdlit.hpp"

#include "SDL.h"
#include "SDL_mixer.h"

#include <cmath>
#include <iostream>

class CloseAudioGuard final {
public:
    CloseAudioGuard() = default;
    ~CloseAudioGuard() { Mix_CloseAudio(); }
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " MUSIC_FILE\n";
        return EXIT_FAILURE;
    }

    sdlit::init(SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512)) {
        std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << '\n';
        return EXIT_FAILURE;
    }
    CloseAudioGuard close_audio_guard{};

    auto music = sdlit::make_unique(Mix_LoadMUS, argv[1]);
    if (not music) {
        std::cerr << "Mix_LoadMUS failed: " << Mix_GetError() << '\n';
        return EXIT_FAILURE;
    }

    if (Mix_PlayMusic(music.get(), 0)) {
        std::cerr << "Mix_PlayMusic failed: " << Mix_GetError() << '\n';
        return EXIT_FAILURE;
    }

    bool running{true};
    SDL_Event event{};
    do {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;
            }
        }
        SDL_Delay(1'000U / 60U);
    } while (running);

    return EXIT_SUCCESS;
}

