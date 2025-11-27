#include"steeringBehavior.h"
#include<algorithm>

clsSteeringBehavior::clsSteeringBehavior(clsMovingEntity* movEnt){
    movEntity = movEnt;  
}

clsVector2d clsSteeringBehavior::seek(clsVector2d targetPos)
{
    clsVector2d desiredVelocity = targetPos.operator-=(movEntity->position);
    desiredVelocity.normalize();
    desiredVelocity.operator*=(movEntity->maxSpeed);
    return desiredVelocity.operator-=(movEntity->velocity);
}

clsVector2d clsSteeringBehavior::flee(clsVector2d targetPos)
{
    clsVector2d desiredVelocity = movEntity->position.operator-=(targetPos);
    desiredVelocity.normalize();
    desiredVelocity.operator*=(movEntity->maxSpeed);
    return desiredVelocity.operator-=(movEntity->velocity);
}

clsVector2d clsSteeringBehavior::arrive(clsVector2d targetPos, Deceleration deceleration){
    clsVector2d vecFromCurrToTarget = targetPos.operator-=(movEntity->position);
    double distance = vecFromCurrToTarget.length();
    const double decelerationTweaker = 0.3;
    if(distance > 0){
        double speed = distance / ((double)deceleration * decelerationTweaker);
        speed = std::min(speed, movEntity->maxSpeed);

        vecFromCurrToTarget.normalize();
        clsVector2d desiredVelocity = vecFromCurrToTarget.operator*=(speed);

        return desiredVelocity.operator-=(movEntity->velocity);
    }
    return clsVector2d(0, 0);
}

