#pragma once 

#include"baseGameEntity.h"
#include"telegram.h"

class clsMovingEntity : public clsBaseGameEntity{
protected:
    clsVector2d velocity;
    double mass;
    double maxSpeed;
    double maxForce;
    double maxTurnRate;//maximum (radians per second) this entity can rotate

    /* Heading and Side vectors represent the
       local coordinate system for the moving entity*/
    //normalized facing vector
    clsVector2d vHeading;
    //a vector perpendicular to the facing vector
    clsVector2d vSide; 

public:
    clsMovingEntity():clsBaseGameEntity(clsBaseGameEntity::nextValidId++){}
    clsMovingEntity(int id);
    void update();//update to the new state
    void render();//draw visual representation of current state
    bool handleMessage(clsTelegram telegram);
};