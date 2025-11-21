#pragma once

#include"playerBase.h"
#include"telegram.h"

class clsGoalKeeper : public clsPlayerBase{
private:
    /* if goalKeeper has the ball,
    the nearby apponent will not attempt to kick it */
    bool goalKeeperHasBall;

public:
    clsGoalKeeper():clsPlayerBase(clsBaseGameEntity::nextValidId++){}
    void update();//update to the new state
    void render();//draw visual representation of current state
    bool handleMessage(clsTelegram telegram);

};