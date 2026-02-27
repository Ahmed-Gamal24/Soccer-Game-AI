#include "../include/goalKeeper.h"
#include "../include/soccerPitch.h"
#include "../include/soccerBall.h"
#include "../include/soccerTeam.h"
#include "../include/steeringBehavior.h"
#include <SDL2/SDL.h>
#include <algorithm>

namespace
{
double clampValue(double v, double minV, double maxV)
{
    return std::max(minV, std::min(v, maxV));
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
    double goalLineX = defendsLeftGoal ? (pitchWidth / 8.0) : ((pitchWidth * 7.0) / 8.0);
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

    // Draw goalkeeper as a larger rectangle (different from field players)
    const double width = 40.0;
    const double height = 60.0;

    double posX = position.getX();
    double posY = position.getY();

    // Draw filled rectangle for goalkeeper
    SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, SDL_ALPHA_OPAQUE);
    SDL_Rect rect = {(int)(posX - width / 2), (int)(posY - height / 2), (int)width, (int)height};
    SDL_RenderFillRect(renderer, &rect);

    // Draw outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Black outline
    SDL_RenderDrawRect(renderer, &rect);
}

bool clsGoalKeeper::handleMessage(clsTelegram telegram)
{
    return false; // TODO: Implement message handling for goalkeeper
}
