#include"movingEntity.h"

clsMovingEntity::clsMovingEntity():clsBaseGameEntity(clsBaseGameEntity::nextValidId++){
    steerBehv = new clsSteeringBehavior(this);
}

clsMovingEntity::clsMovingEntity(int id):clsBaseGameEntity(id){
    steerBehv = new clsSteeringBehavior(this);
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

