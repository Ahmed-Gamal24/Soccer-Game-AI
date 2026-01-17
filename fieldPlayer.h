#pragma once

#include "playerBase.h"
#include "telegram.h"
#include "SDL3/SDL.h"
#include "stateMachine.h"

class clsFieldPlayer : public clsPlayerBase
{
public:
    clsStateMachine<clsFieldPlayer> stateMachine;

    clsFieldPlayer();
    void update();                       // update to the new state
    void render(SDL_Renderer *renderer); // draw visual representation of current state
    bool handleMessage(clsTelegram telegram);
};