#include "SDLit.hpp"

#include "SDL.h"
#include "SDL_mixer.h"

#include <cmath>
#include <iostream>

struct Context {
    float wave_frequency{};
    float wave_phase{};
    SDL_AudioSpec desired{};
    SDL_AudioSpec obtained{};

    Context() {
        wave_frequency = 240.0f;

        desired.freq = 48'000;
        desired.format = AUDIO_F32;
        desired.channels = 1;
        desired.samples = 4096;
        desired.userdata = this;
    }
};

void sineWaveCallback(void* userdata, Uint8* stream, int len) {
    Context* const context = static_cast<Context*>(userdata);
    float* samples = reinterpret_cast<float*>(stream);
    int samples_len = len / sizeof(float);

    for (int i{}; i < samples_len; ++i) {
        samples[i] = std::sin(context->wave_phase * M_PI);
        context->wave_phase += context->wave_frequency / static_cast<float>(context->obtained.freq);
        if (context->wave_phase >= 1.0) {
            context->wave_phase -= 1.0;
        }
        std::cout << context->wave_phase << " " << samples[i] <<  std::endl;
    }
}

int main(int argc, char** argv) {
    SDLit::init(SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    constexpr std::int32_t kFrequency{44'100};
    constexpr std::uint16_t kFormat{AUDIO_S16SYS};
    constexpr std::int32_t kStereo{2};
    constexpr std::int32_t kSampleCount{256};
    constexpr std::int32_t kChunkSize{kStereo * kSampleCount};

    Context context{};
    context.desired.callback = sineWaveCallback;
    SDL_AudioDeviceID audio_device_id = SDL_OpenAudioDevice(nullptr, 0, &context.desired, &context.obtained, 0U);
    if (audio_device_id <= 0) {
        std::cerr << "SDL_OpenAudioDevice failed: " << Mix_GetError() << '\n';
        return EXIT_FAILURE;
    }
    SDL_PauseAudioDevice(audio_device_id, 0);

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

