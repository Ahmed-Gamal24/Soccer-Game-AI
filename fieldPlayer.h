#pragma once

#include "playerBase.h"
#include "telegram.h"

class clsFieldPlayer : public clsPlayerBase {

public:
    clsFieldPlayer();
    void update(); // update to the new state
    void render(); // draw visual representation of current state
    bool handleMessage(clsTelegram telegram);
};