#include "../include/steeringBehavior.h"
#include "../include/movingEntity.h"
#include "../include/soccerBall.h"
#include "../include/playerBase.h"
#include "../include/soccerPitch.h"
#include <algorithm>

clsSteeringBehavior::clsSteeringBehavior(clsPlayerBase *player)
{
    this->player = player;
    this->isSeeking = false;
    this->isArriving = false;
    this->currentDeceleration = Deceleration::normal;
    this->targetPos = clsVector2d(0, 0);
}

clsVector2d clsSteeringBehavior::seek(clsVector2d targetPos)
{
    this->targetPos = targetPos;
    this->isSeeking = true;
    this->isArriving = false;

    clsVector2d toTarget = targetPos;
    toTarget.operator-=(player->position);
    toTarget.normalize();
    toTarget.operator*=(player->maxSpeed);

    clsVector2d steeringForce = toTarget;
    steeringForce.operator-=(player->velocity);
    return steeringForce;
}

clsVector2d clsSteeringBehavior::arrive(clsVector2d targetPos, Deceleration deceleration)
{
    this->targetPos = targetPos;
    this->isArriving = true;
    this->isSeeking = false;
    this->currentDeceleration = deceleration;

    clsVector2d vecFromCurrToTarget = targetPos;
    vecFromCurrToTarget.operator-=(player->position);
    double distance = vecFromCurrToTarget.length();
    const double decelerationTweaker = 0.3;
    if (distance > 0)
    {
        double speed = distance / ((double)deceleration * decelerationTweaker);
        speed = std::min(speed, player->maxSpeed);

        vecFromCurrToTarget.normalize();
        clsVector2d desiredVelocity = vecFromCurrToTarget;
        desiredVelocity.operator*=(speed);

        clsVector2d steeringForce = desiredVelocity;
        steeringForce.operator-=(player->velocity);
        return steeringForce;
    }
    return clsVector2d(0, 0);
}

clsVector2d clsSteeringBehavior::calculate()
{
    clsVector2d steeringForce(0, 0);

    // Apply the currently active steering behavior
    if (isSeeking)
    {
        steeringForce = seek(targetPos);
        isSeeking = false; // Reset flag for next frame
    }
    else if (isArriving)
    {
        steeringForce = arrive(targetPos, (clsSteeringBehavior::Deceleration)currentDeceleration);
        isArriving = false; // Reset flag for next frame
    }
    // If no behavior is active, steering force remains (0, 0)

    steeringForce.truncate(MAXSTEERINGFORCE);
    return steeringForce;
}

clsVector2d clsSteeringBehavior::pursuit(clsMovingEntity *evader)
{
    // Compute vector to the evader
    clsVector2d toEvader = evader->position;
    toEvader.operator-=(player->position);

    double relativeHeading = player->vHeading.dot(evader->vHeading);

    // If the evader is ahead and facing us, then seek its current position
    if ((toEvader.dot(player->vHeading) > 0) && (relativeHeading < -0.95))
    {
        // Use the same steering logic as seek but return the steering force directly
        clsVector2d desired = evader->position;
        desired.operator-=(player->position);
        if (!desired.isZero())
        {
            desired.normalize();
            desired.operator*=(player->maxSpeed);
        }
        clsVector2d steering = desired;
        steering.operator-=(player->velocity);
        steering.truncate(MAXSTEERINGFORCE);
        return steering;
    }

    // Not heading directly towards us — predict future position of the evader
    double distance = toEvader.length();
    double speed = player->maxSpeed + evader->velocity.length();
    double lookAheadTime = 0.0;
    if (speed > 0.0001)
    {
        lookAheadTime = distance / speed;
    }

    // Predict future position
    clsVector2d futurePos = evader->position;
    clsVector2d evVel = evader->velocity;
    evVel.operator*=(lookAheadTime);
    futurePos.operator+=(evVel);

    // Compute steering towards predicted position
    clsVector2d desired = futurePos;
    desired.operator-=(player->position);
    if (!desired.isZero())
    {
        desired.normalize();
        desired.operator*=(player->maxSpeed);
    }

    clsVector2d steering = desired;
    steering.operator-=(player->velocity);
    steering.truncate(MAXSTEERINGFORCE);
    return steering;
}
