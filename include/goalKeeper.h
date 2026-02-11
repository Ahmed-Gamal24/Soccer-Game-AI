#pragma once

#include "playerBase.h"
#include "telegram.h"
#include <SDL2/SDL.h>

class clsGoalKeeper : public clsPlayerBase
{
private:
    /* if goalKeeper has the ball,
    the nearby apponent will not attempt to kick it */
    bool goalKeeperHasBall;

public:
    // Color properties (RGB)
    uint8_t colorR = 200;
    uint8_t colorG = 200;
    uint8_t colorB = 0;

    clsGoalKeeper() : clsPlayerBase(clsBaseGameEntity::nextValidId++) {}
    void update();                       // update to the new state
    void render(SDL_Renderer *renderer); // draw visual representation of current state
    bool handleMessage(clsTelegram telegram);

    // Color setter
    void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
        colorR = r;
        colorG = g;
        colorB = b;
    }
};