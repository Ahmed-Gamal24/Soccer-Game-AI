#include "../include/goalKeeper.h"
#include "../include/soccerPitch.h"
#include "../include/soccerBall.h"
#include "../include/soccerTeam.h"
#include "../include/steeringBehavior.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>

namespace
{
double clampValue(double v, double minV, double maxV)
{
    return std::max(minV, std::min(v, maxV));
}

void drawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius)
{
    for (int dy = -radius; dy <= radius; ++dy)
    {
        int span = (int)std::sqrt((double)(radius * radius - dy * dy));
        SDL_RenderDrawLine(renderer, cx - span, cy + dy, cx + span, cy + dy);
    }
}
}

void clsGoalKeeper::update()
{
    if (soccerPitch == nullptr)
        return;

    clsSoccerBall *ball = soccerPitch->getBall();
    if (ball == nullptr)
        return;

    double pitchWidth = soccerPitch->getWidth();
    double pitchHeight = soccerPitch->getHeight();

    // Keepers slide on their own goal line and track the ball vertically.
    bool defendsLeftGoal = position.getX() < pitchWidth / 2.0;
    double goalLineX = defendsLeftGoal ? 2.0 : (pitchWidth - 2.0);
    double minGoalY = pitchHeight / 3.0;
    double maxGoalY = (pitchHeight * 2.0) / 3.0;
    double targetY = clampValue(ball->position.getY(), minGoalY, maxGoalY);

    if (steerBehv != nullptr)
    {
        steerBehv->arrive(clsVector2d(goalLineX, targetY), clsSteeringBehavior::Deceleration::normal);
    }

    // Move goalkeeper.
    clsMovingEntity::update(1.0 / 60.0);

    // Trap very close free balls in the box.
    goalKeeperHasBall = false;
    if (ball->getBallOwner() == nullptr && position.distance(ball->position) < 45.0)
    {
        ball->trap(this);
        goalKeeperHasBall = true;

        clsSoccerTeam *redTeam = soccerPitch->getRedTeam();
        clsSoccerTeam *blueTeam = soccerPitch->getBlueTeam();

        if (redTeam != nullptr && redTeam->getGoalKeeper() == this)
        {
            redTeam->setControllingPlayer(this);
            if (blueTeam != nullptr)
                blueTeam->setControllingPlayer(nullptr);
        }
        else if (blueTeam != nullptr && blueTeam->getGoalKeeper() == this)
        {
            blueTeam->setControllingPlayer(this);
            if (redTeam != nullptr)
                redTeam->setControllingPlayer(nullptr);
        }
    }
}

void clsGoalKeeper::render(SDL_Renderer *renderer)
{
    if (!renderer)
        return;

    int cx = (int)position.getX();
    int cy = (int)position.getY();
    int radius = 24;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
    drawFilledCircle(renderer, cx + 2, cy + 3, radius);

    SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, SDL_ALPHA_OPAQUE);
    drawFilledCircle(renderer, cx, cy, radius);

    SDL_SetRenderDrawColor(renderer, 15, 15, 15, SDL_ALPHA_OPAQUE);
    for (int a = 0; a < 360; ++a)
    {
        double rad = a * 0.017453292519943295;
        int x = cx + (int)(std::cos(rad) * radius);
        int y = cy + (int)(std::sin(rad) * radius);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    clsVector2d heading = vHeading;
    if (heading.isZero())
        heading = clsVector2d(0.0, 1.0);
    else
        heading.normalize();

    clsVector2d side = heading.perp();
    side.normalize();

    int gx1 = cx + (int)(side.getX() * (radius - 3));
    int gy1 = cy + (int)(side.getY() * (radius - 3));
    int gx2 = cx - (int)(side.getX() * (radius - 3));
    int gy2 = cy - (int)(side.getY() * (radius - 3));

    SDL_SetRenderDrawColor(renderer, 245, 245, 245, SDL_ALPHA_OPAQUE);
    drawFilledCircle(renderer, gx1, gy1, 5);
    drawFilledCircle(renderer, gx2, gy2, 5);
}

bool clsGoalKeeper::handleMessage(clsTelegram telegram)
{
    return false; // TODO: Implement message handling for goalkeeper
}
