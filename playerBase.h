#pragma once

#include"movingEntity.h"
#include"vector2d.h"

// Forward declarations
class clsFieldPlayer;
class clsSteeringBehavior;

class clsPlayerBase: public clsMovingEntity{

private:
    clsVector2d playerHomeRegion;

public:
    clsPlayerBase();
    clsPlayerBase(int id);
    bool canPassForward(clsFieldPlayer* receiver, clsFieldPlayer* target, int power);
    bool canPassBackward(clsFieldPlayer* receiver, clsFieldPlayer* target, int power);
    bool withinShootingRange();
    bool atTarget();

    void setPlayerHomeRegion(clsVector2d regCenter);
    clsVector2d getPlayerHomeRegion();
};