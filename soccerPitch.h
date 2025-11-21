#pragma once 

#include <SDL3/SDL.h>
#include <vector>
#include <iostream>
#include"soccerTeam.h"
#include"soccerBall.h"
#include"goal.h"
#include"region.h"
#include"boundryWall.h"


class clsSoccerPitch{
private:
    //dimentions of the stadium
    double _width;
    double _height;
    bool _gameOn; 

    clsWall* boundries;

    clsGoal* redGoal;
    clsGoal* blueGoal;

    // clsSoccerTeam* redTeam;
    // clsSoccerTeam* blueTeam;

    clsSoccerBall* ball;


public:
    /* the update and render functions here in the ptich class 
       used to call all the update and render functions of all entities
    */
    clsSoccerPitch();
    double getWidth();
    double getHeight();
    void update();//update to the new state
    void render();//draw visual representation of current state

};