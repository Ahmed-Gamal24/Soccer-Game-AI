#include "playerBase.h"
#include "fieldPlayer.h"
#include "steeringBehavior.h"

clsPlayerBase::clsPlayerBase() : clsMovingEntity(clsBaseGameEntity::nextValidId++,
                                                 clsVector2d(0, 0), 10, clsVector2d(1, 1), 1, 300, 500, 10)
{
    // Initialize steering behavior for player
    steerBehv = new clsSteeringBehavior(this);
}

clsPlayerBase::clsPlayerBase(int id) : clsMovingEntity(id,
                                                       clsVector2d(0, 0), 10, clsVector2d(1, 1), 1, 300, 500, 10)
{
    // Initialize steering behavior for player
    steerBehv = new clsSteeringBehavior(this);
}

bool clsPlayerBase::canPassForward(clsFieldPlayer *receiver, clsFieldPlayer *target, int power)
{
    return false; // TODO: Implement
}

bool clsPlayerBase::canPassBackward(clsFieldPlayer *receiver, clsFieldPlayer *target, int power)
{
    return false; // TODO: Implement
}

bool clsPlayerBase::withinShootingRange()
{
    return false; // TODO: Implement - check if player is within shooting range of goal
}

bool clsPlayerBase::atTarget()
{
    return false; // TODO: Implement - check if player has reached target position
}

void clsPlayerBase::setPlayerHomeRegion(clsVector2d regCenter)
{
    playerHomeRegion = regCenter;
}
clsVector2d clsPlayerBase::getPlayerHomeRegion()
{
    return playerHomeRegion;
}
