#include "../include/fieldPlayer.h"
#include "../include/fieldPlayerStates.h"
#include <cmath>

namespace
{
void drawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius)
{
    for (int dy = -radius; dy <= radius; ++dy)
    {
        int span = (int)std::sqrt((double)(radius * radius - dy * dy));
        SDL_RenderDrawLine(renderer, cx - span, cy + dy, cx + span, cy + dy);
    }
}
}

clsFieldPlayer::clsFieldPlayer()
    : clsPlayerBase(clsBaseGameEntity::nextValidId++),
      stateMachine(this)
{
    stateMachine.setGlobalState(new GlobalPlayerState());
    stateMachine.setCurrentState(new Wait());
}

void clsFieldPlayer::update()
{
    // First update the state machine to execute player behaviors (which set steering targets)
    stateMachine.updateStateMachine();

    // Then apply physics (steering forces, movement)
    clsMovingEntity::update(1.0 / 60.0); // 60 FPS
}

void clsFieldPlayer::render(SDL_Renderer *renderer)
{
    if (!renderer)
        return;

    int cx = (int)position.getX();
    int cy = (int)position.getY();
    int radius = 18;

    // Subtle drop shadow.
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 70);
    drawFilledCircle(renderer, cx + 2, cy + 3, radius);

    // Body.
    SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, SDL_ALPHA_OPAQUE);
    drawFilledCircle(renderer, cx, cy, radius);

    // Outline.
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
    for (int a = 0; a < 360; ++a)
    {
        double rad = a * 0.017453292519943295;
        int x = cx + (int)(std::cos(rad) * radius);
        int y = cy + (int)(std::sin(rad) * radius);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    // Facing indicator.
    clsVector2d heading = vHeading;
    if (heading.isZero())
    {
        heading = clsVector2d(1.0, 0.0);
    }
    else
    {
        heading.normalize();
    }

    int hx = cx + (int)(heading.getX() * (radius + 10));
    int hy = cy + (int)(heading.getY() * (radius + 10));
    SDL_SetRenderDrawColor(renderer, 250, 250, 250, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, cx, cy, hx, hy);
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, SDL_ALPHA_OPAQUE);
    drawFilledCircle(renderer, hx, hy, 3);
}

bool clsFieldPlayer::handleMessage(clsTelegram telegram)
{
    return stateMachine.handleMessage(telegram);
}
