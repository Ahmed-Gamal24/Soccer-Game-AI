#pragma once

#include "vector2d.h"

// Forward declarations
class clsMovingEntity;
class clsSoccerBall;
class clsPlayerBase;
class clsSoccerPitch;

class clsSteeringBehavior
{
public:
    enum Deceleration
    {
        slow = 3,
        normal = 2,
        fast = 1
    };

private:
    clsSoccerBall *ball;
    clsPlayerBase *player;
    clsSoccerPitch *pitch;

    // Current steering targets and behavior
    clsVector2d targetPos;
    bool isSeeking;
    bool isArriving;
    Deceleration currentDeceleration;

public:
    clsSteeringBehavior(clsPlayerBase *player);

    // seek function generate the required force to reach the tartgetPos
    clsVector2d seek(clsVector2d targetPos);

    clsVector2d arrive(clsVector2d targetPos, Deceleration deceleration);

    const double MAXSTEERINGFORCE = 100;
    clsVector2d calculate();

    clsVector2d pursuit(clsMovingEntity *evader);
};