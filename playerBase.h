#pragma once

#include"movingEntity.h"


class clsPlayerBase: public clsMovingEntity{

private:
    

public:
    clsPlayerBase():clsMovingEntity(clsBaseGameEntity::nextValidId++){}
    clsPlayerBase(int id);
    // bool canPassForward(clsFieldPlayer receiver,clsFieldPlayer target, int power);
    // bool canPassBackward(clsFieldPlayer receiver,clsFieldPlayer target, int power);
    bool withinShootingRange();
    bool atTarget();
};