#include "soccerBall.h"
#include "math.h"

clsSoccerBall::clsSoccerBall(clsVector2d position, double ballSize,
                  double mass, clsWall* pitchWalls):clsMovingEntity(clsBaseGameEntity::nextValidId++){
    
}

void clsSoccerBall::trap(clsPlayerBase *owner)
{
    velocity.zero();
    ballOwner = owner;
}

clsVector2d clsSoccerBall::getOldPos()
{
    return oldPosition;
}


void clsSoccerBall::render(SDL_Renderer *renderer)
{
    // simple filled circle using horizontal spans (midpoint circle algorithm)
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;

    int cx = static_cast<int>(std::round(position.getX()));
    int cy = static_cast<int>(std::round(position.getY()));

    while (x >= y)
    {
        // top half
        SDL_RenderLine(renderer, cx - x, cy - y, cx + x, cy - y);
        SDL_RenderLine(renderer, cx - y, cy - x, cx + y, cy - x);

        // bottom half
        SDL_RenderLine(renderer, cx - x, cy + y, cx + x, cy + y);
        SDL_RenderLine(renderer, cx - y, cy + x, cx + y, cy + x);

        y++;
        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        }
        else
        {
            x--;
            radiusError += 2 * (y - x) + 1;
        }
    }
}