#pragma once

#include"movingEntity.h"
#include"vector2d.h"
#include"playerBase.h"
#include"boundryWall.h"
#include"SDL3/SDL.h"

class clsSoccerBall : public clsMovingEntity{

private:
    clsVector2d oldPosition;
    clsPlayerBase* ballOwner;
    clsWall* pitchWalls;//to test collision between ball and walls

public:
    clsSoccerBall(clsVector2d position, double radius,
                 double ballSize, double mass, clsWall* pitchWalls);
    void update(double time_elapsed);//update to the new state
    void render(SDL_Renderer* renderer);//draw visual representation of current state
    void kick(int clsVector2d, int force);
    
    double fricion = -0.015;
    clsVector2d futurePosition(double time);
    void trap(clsPlayerBase* owner);
    // calc the time taken to move from position a to position b based on that force
    double timeToCoverDistance(clsVector2d A, clsVector2d B, double force)const;
    void testCollisionWithWalls(clsWall* pitchWalls);//if collision happened reflect the ball's velocity
    clsVector2d getOldPos();
    void placeBallAtPosition(clsVector2d newPos);
};