#include "goalKeeper.h"
#include "SDL3/SDL.h"

void clsGoalKeeper::update() {}

void clsGoalKeeper::render(SDL_Renderer *renderer)
{
    if (!renderer)
        return;

    // Draw goalkeeper as a larger rectangle (different from field players)
    const double width = 40.0;
    const double height = 60.0;

    double posX = position.getX();
    double posY = position.getY();

    // Draw filled rectangle for goalkeeper
    SDL_SetRenderDrawColor(renderer, 200, 200, 0, SDL_ALPHA_OPAQUE); // Yellow-green color
    SDL_FRect rect = {(float)(posX - width / 2), (float)(posY - height / 2), (float)width, (float)height};
    SDL_RenderFillRect(renderer, &rect);

    // Draw outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Black outline
    SDL_RenderRect(renderer, &rect);
}

bool clsGoalKeeper::handleMessage(clsTelegram telegram)
{
    return false; // TODO: Implement message handling for goalkeeper
}