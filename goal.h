#pragma once 

#include"baseGameEntity.h"
#include"soccerBall.h"
#include <SDL3/SDL.h>


class clsGoal : clsBaseGameEntity{
private:
    
    //using the leftPost point and the rightPost point you see the line of the goal
    clsVector2d leftPost;
    clsVector2d rightPost;

    clsVector2d centerOfGoalLine;

    int numGoalScored;

public:
    clsGoal(clsVector2d left, clsVector2d right);
    clsVector2d getLeftPost();
    clsVector2d getRightPost();
    bool isScored(clsSoccerBall* ball);
    void render(SDL_Renderer* renderer);

};