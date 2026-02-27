#include "../include/soccerPitch.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;

int main()
{

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG); // Initialize Image library

    int windowWidth = 2000;
    int windowHeight = 1000;

    SDL_Window *window = SDL_CreateWindow("Soccer AI Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create the soccerPitch with renderer
    clsSoccerPitch soccerPitch(renderer,windowWidth , windowHeight);

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

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
