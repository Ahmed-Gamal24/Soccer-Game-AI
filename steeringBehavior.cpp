#include"steeringBehavior.h"
#include<algorithm>

clsSteeringBehavior::clsSteeringBehavior(clsPlayerBase* player){
    this->player = player;
}

clsVector2d clsSteeringBehavior::seek(clsVector2d targetPos)
{
    clsVector2d desiredVelocity = targetPos.operator-=(player->position);
    desiredVelocity.normalize();
    desiredVelocity.operator*=(player->maxSpeed);
    return desiredVelocity.operator-=(player->velocity);
}


clsVector2d clsSteeringBehavior::arrive(clsVector2d targetPos, Deceleration deceleration){
    clsVector2d vecFromCurrToTarget = targetPos.operator-=(player->position);
    double distance = vecFromCurrToTarget.length();
    const double decelerationTweaker = 0.3;
    if(distance > 0){
        double speed = distance / ((double)deceleration * decelerationTweaker);
        speed = std::min(speed, player->maxSpeed);

        vecFromCurrToTarget.normalize();
        clsVector2d desiredVelocity = vecFromCurrToTarget.operator*=(speed);

        return desiredVelocity.operator-=(player->velocity);
    }
    return clsVector2d(0, 0);
}

clsVector2d clsSteeringBehavior::calculate(){
    clsVector2d steeringForce;
    steeringForce.operator+=();
    steeringForce.operator+=();
    steeringForce.operator+=();

    steeringForce.truncate(MAXSTEERINGFORCE);
    return steeringForce;
}

clsVector2d clsSteeringBehavior::pursuit(clsMovingEntity* evader){
    //if evader is facing the pursuiter: just seek the current position of evader
    clsVector2d toEvader = evader->position.operator-=(player->position);
    double relativeHeading = player->vHeading.dot(evader->vHeading);

    if ((toEvader.dot(player->vHeading) > 0) &&
    (relativeHeading < -0.95)) //if the angle less than 18 degs, we consider the evader is ahead
    {
        return seek(evader->position);  
    }
    
  
    //if evader is not facing the pursuiter

}


