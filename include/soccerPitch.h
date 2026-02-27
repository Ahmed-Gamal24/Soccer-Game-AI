#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include "region.h"
#include "boundryWall.h"

// Forward declarations
class clsSoccerTeam;
class clsSoccerBall;
class clsGoal;

class clsSoccerPitch
{
private:
    // dimentions of the stadium
    double _width;
    double _height;
    bool _gameOn;

    SDL_Renderer *_renderer;

    clsWall *boundries;

    clsGoal *redGoal;
    clsGoal *blueGoal;

    clsSoccerTeam *redTeam;
    clsSoccerTeam *blueTeam;

    clsSoccerBall *ball;

    std::vector<clsRegion> regions;

    // Scoring system
    int redScore;
    int blueScore;
    const int WINNING_SCORE = 3;

public:
    /* the update and render functions here in the ptich class
       used to call all the update and render functions of all entities
    */
    clsSoccerPitch(SDL_Renderer *renderer, int w, int h);
    double getWidth();
    double getHeight();
    clsSoccerBall *getBall();
    clsSoccerTeam *getRedTeam();
    clsSoccerTeam *getBlueTeam();

    // Scoring and game state
    int getRedScore() const { return redScore; }
    int getBlueScore() const { return blueScore; }
    int getWinningScore() const { return WINNING_SCORE; }
    void resetGame();

    void update(); // update to the new state
    void render(); // draw visual representation of current state
};
