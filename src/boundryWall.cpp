#include "../include/boundryWall.h"

clsWall::clsWall()
{
    clsVector2d zero;
    firstPoint = zero;
    secondPoint = zero;
}
clsWall::clsWall(clsVector2d fPoint, clsVector2d sPoint)
{
    firstPoint = fPoint;
    secondPoint = sPoint;
}

void clsWall::setFPoint(clsVector2d fpoint)
{
    firstPoint = fpoint;
}

void clsWall::setSPoint(clsVector2d spoint)
{
    secondPoint = spoint;
}

void clsWall::setWall(clsVector2d fPoint, clsVector2d sPoint)
{
    setFPoint(fPoint);
    setSPoint(sPoint);
}

void clsWall::render(SDL_Renderer *renderer)
{
    SDL_RenderDrawLine(renderer, (int)firstPoint.getX(), (int)firstPoint.getY(), (int)secondPoint.getX(), (int)secondPoint.getY());
}
