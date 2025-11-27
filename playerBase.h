#pragma once

#include"movingEntity.h"
#include"fieldPlayer.h"


class clsPlayerBase: public clsMovingEntity{

private:
    

public:
    clsPlayerBase();
    clsPlayerBase(int id);
    bool canPassForward(clsFieldPlayer receiver,clsFieldPlayer target, int power);
    bool canPassBackward(clsFieldPlayer receiver,clsFieldPlayer target, int power);
    bool withinShootingRange();
    bool atTarget();
};