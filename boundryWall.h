#pragma once 

#include"vector2d.h"
#include"SDL3\SDL.h"

class clsWall{ //wall(line) is defined by two points(2dvector)

private:
     
    clsVector2d firstPoint;
    clsVector2d secondPoint;

public:
    clsWall();
    clsWall(clsVector2d fPoint, clsVector2d sPoint);
    void setFPoint(clsVector2d fpoint);
    void setSPoint(clsVector2d fpoint);
    void setWall(clsVector2d fPoint, clsVector2d sPoint);
    void render(SDL_Renderer* renderer);

};