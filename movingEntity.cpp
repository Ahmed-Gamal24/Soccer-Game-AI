#include"movingEntity.h"

clsMovingEntity::clsMovingEntity(clsVector2d position, double radius,
                         clsVector2d scale, double mass,
                         double maxSpeed, double maxForce, double maxTurnRate):
                         clsBaseGameEntity(clsBaseGameEntity::nextValidId++, position, radius, scale)
{
    steerBehv = new clsSteeringBehavior(this);
    this->mass = mass;
    this->maxSpeed = maxSpeed;
    this->maxForce = maxForce;
    this->maxTurnRate = maxTurnRate;
}

clsMovingEntity::clsMovingEntity(int id, clsVector2d position, double radius,
                     clsVector2d scale, double mass, double maxSpeed,
                     double maxForce, double maxTurnRate):
                     clsBaseGameEntity(id, position, radius, scale)
{
    steerBehv = new clsSteeringBehavior(this);
    this->mass = mass;
    this->maxSpeed = maxSpeed;
    this->maxForce = maxForce;
    this->maxTurnRate = maxTurnRate;
}

void clsMovingEntity::wrapAround(clsVector2d position, double width, double height){

}

void clsMovingEntity::update(double time_elapsed){
    clsVector2d steeringForce = steerBehv->calculate();
    clsVector2d acceleration = steeringForce.operator/=(this->mass);
    this->velocity.operator+=(acceleration.operator*=(time_elapsed));
    velocity.Truncate(this->maxSpeed);
    this->position.operator+=(velocity.operator*=(time_elapsed));

    if(velocity.squaredLength() > EPSILON){
        this->velocity.normalize();
        this->vHeading.operator=(this->velocity);
        this->vSide.operator=(this->vHeading.perp());
    }

    wrapAround(this->position, soccerPitch->getWidth(), soccerPitch->getHeight());
}

void clsMovingEntity::render(){}

bool clsMovingEntity::handleMessage(clsTelegram telegram){}

