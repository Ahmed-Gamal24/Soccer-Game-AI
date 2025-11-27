#pragma once 

#include"baseGameEntity.h"
#include"telegram.h"
#include"soccerPitch.h"
#include"steeringBehavior.h"


class clsMovingEntity : public clsBaseGameEntity{
public:
    double mass;
    double maxSpeed;
    double maxForce;//maximum force this entity can produce to power itself
    double maxTurnRate;//maximum (radians per second) this entity can rotate
    
    clsVector2d velocity;

    /* Heading and Side vectors represent the
    local coordinate system for the moving entity*/
    //normalized facing vector
    clsVector2d vHeading;

    //a vector perpendicular to the facing vector
    clsVector2d vSide; 

    clsSoccerPitch* soccerPitch; //so the moving entity can see the game world and it's wall
    
    clsSteeringBehavior* steerBehv;

    clsMovingEntity();
    clsMovingEntity(int id);

    //function to check if the position exceeds the screen boundries to wrap around it
    void wrapAround(clsVector2d position, double width, double height);
    void update(double time_elapsed);//update to the new state
    void render();//draw visual representation of current state
    bool handleMessage(clsTelegram telegram);
};