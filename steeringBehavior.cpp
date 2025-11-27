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

clsVector2d clsSteeringBehavior::calculate(){
    clsVector2d steeringForce;
    steeringForce.operator+=();
    steeringForce.operator+=();
    steeringForce.operator+=();

    steeringForce.Truncate(MAXSTEERINGFORCE);
    return steeringForce;
}

clsVector2d clsSteeringBehavior::pursuit(clsMovingEntity* evader){
    //if evader is facing the pursuiter: just seek the current position of evader
    clsVector2d toEvader = evader->position.operator-=(movEntity->position);
    double relativeHeading = movEntity->vHeading.dot(evader->vHeading);

    if ((toEvader.dot(movEntity->vHeading) > 0) &&
    (relativeHeading < -0.95)) //if the angle less than 18 degs, we consider the evader is ahead
    {
        return seek(evader->position);  
    }
    
  
    //if evader is not facing the pursuiter

}
