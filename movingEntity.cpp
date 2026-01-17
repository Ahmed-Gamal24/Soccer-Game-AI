#include "movingEntity.h"
#include "soccerPitch.h"
#include "steeringBehavior.h"

clsMovingEntity::clsMovingEntity(clsVector2d position, double radius,
                                 clsVector2d scale, double mass,
                                 double maxSpeed, double maxForce, double maxTurnRate) : clsBaseGameEntity(clsBaseGameEntity::nextValidId++, position, radius, scale)
{
    // Steering behavior will be set by derived classes (PlayerBase sets it properly)
    steerBehv = nullptr;
    this->mass = mass;
    this->maxSpeed = maxSpeed;
    this->maxForce = maxForce;
    this->maxTurnRate = maxTurnRate;
}

clsMovingEntity::clsMovingEntity(int id, clsVector2d position, double radius,
                                 clsVector2d scale, double mass, double maxSpeed,
                                 double maxForce, double maxTurnRate) : clsBaseGameEntity(id, position, radius, scale)
{
    // Steering behavior will be set by derived classes (PlayerBase sets it properly)
    steerBehv = nullptr;
    this->mass = mass;
    this->maxSpeed = maxSpeed;
    this->maxForce = maxForce;
    this->maxTurnRate = maxTurnRate;
}

void clsMovingEntity::wrapAround(clsVector2d position, double width, double height)
{
}

void clsMovingEntity::update(double time_elapsed)
{
    if (steerBehv != nullptr)
    {
        clsVector2d steeringForce = steerBehv->calculate();
        clsVector2d acceleration = steeringForce;
        acceleration.operator/=(this->mass);
        clsVector2d velChange = acceleration;
        velChange.operator*=(time_elapsed);
        this->velocity.operator+=(velChange);
        velocity.truncate(this->maxSpeed);
    }

    clsVector2d posChange = velocity;
    posChange.operator*=(time_elapsed);
    this->position.operator+=(posChange);

    if (velocity.squaredLength() > EPSILON)
    {
        clsVector2d velCopy = velocity;
        velCopy.normalize();
        this->vHeading.operator=(velCopy);
        this->vSide = this->vHeading.perp();
    }

    if (soccerPitch != nullptr)
    {
        wrapAround(this->position, soccerPitch->getWidth(), soccerPitch->getHeight());
    }
}

void clsMovingEntity::render() {}

bool clsMovingEntity::handleMessage(clsTelegram telegram)
{
    return false;
}
