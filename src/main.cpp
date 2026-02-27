#include "../include/soccerPitch.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct LoopingAudioState
{
    Uint8 *buffer = nullptr;
    Uint32 length = 0;
    Uint32 position = 0;
};

void audioCallback(void *userdata, Uint8 *stream, int len)
{
    LoopingAudioState *state = static_cast<LoopingAudioState *>(userdata);
    if (state == nullptr || state->buffer == nullptr || state->length == 0)
    {
        SDL_memset(stream, 0, len);
        return;
    }

    int remaining = len;
    while (remaining > 0)
    {
        Uint32 chunk = state->length - state->position;
        if (chunk > static_cast<Uint32>(remaining))
        {
            chunk = static_cast<Uint32>(remaining);
        }

        SDL_memcpy(stream + (len - remaining), state->buffer + state->position, chunk);
        state->position = (state->position + chunk) % state->length;
        remaining -= static_cast<int>(chunk);
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        cout << "SDL_Init failed: " << SDL_GetError() << endl;
        return 1;
    }
    IMG_Init(IMG_INIT_PNG); 

    int windowWidth = 2000;
    int windowHeight = 1000;

    SDL_Window *window = SDL_CreateWindow("Soccer AI Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create the soccerPitch with renderer
    clsSoccerPitch soccerPitch(renderer,windowWidth , windowHeight);

    // Optional looping audience ambience from WAV file.
    LoopingAudioState crowdAudio;
    SDL_AudioDeviceID audioDevice = 0;
    std::vector<std::string> audioPaths = {
        "audio/crowd.wav"
    };

    SDL_AudioSpec wavSpec;
    Uint8 *wavBuffer = nullptr;
    Uint32 wavLength = 0;

    for (const auto &path : audioPaths)
    {
        if (SDL_LoadWAV(path.c_str(), &wavSpec, &wavBuffer, &wavLength) != nullptr)
        {
            crowdAudio.buffer = wavBuffer;
            crowdAudio.length = wavLength;
            crowdAudio.position = 0;

            wavSpec.callback = audioCallback;
            wavSpec.userdata = &crowdAudio;

            audioDevice = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);
            if (audioDevice != 0)
            {
                SDL_PauseAudioDevice(audioDevice, 0); // Start playback.
                cout << "Playing crowd ambience: " << path << endl;
            }
            else
            {
                cout << "Audio device open failed: " << SDL_GetError() << endl;
            }
            break;
        }
    }

    if (audioDevice == 0)
    {
        cout << "No crowd audio found. Add a WAV file at audio/crowd.wav" << endl;
    }

    bool isRunning = true;
    SDL_Event event;

    // Run slower than uncapped CPU speed so gameplay flow is observable.
    const int targetFPS = 120;
    const Uint32 targetFrameMs = 1000 / targetFPS;

    while (isRunning)
    {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
                isRunning = false;
        }

        soccerPitch.update();
        soccerPitch.render();

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < targetFrameMs)
        {
            SDL_Delay(targetFrameMs - frameTime);
        }
    }

    if (audioDevice != 0)
    {
        SDL_CloseAudioDevice(audioDevice);
    }
    if (wavBuffer != nullptr)
    {
        SDL_FreeWAV(wavBuffer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
