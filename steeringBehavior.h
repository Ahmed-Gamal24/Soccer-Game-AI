#include"vector2d.h"
#include"movingEntity.h"

class clsSteeringBehavior{
    private:
        clsMovingEntity* movEntity;

    public:
        clsSteeringBehavior(clsMovingEntity* movEnt);

        //seek function generate the required force to reach the tartgetPos
        clsVector2d seek(clsVector2d targetPos);
        
        //flee function create force to steer the agent away
        clsVector2d flee(clsVector2d targetPos);

        enum Deceleration{slow = 3, normal = 2, fast = 1};
        clsVector2d arrive(clsVector2d targetPos, Deceleration deceleration);

        const double MAXSTEERINGFORCE;
        clsVector2d calculate();

        clsVector2d pursuit(clsMovingEntity* evader);

};