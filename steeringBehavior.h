#include"vector2d.h"
#include"movingEntity.h"
#include"soccerBall.h"
#include"playerBase.h"
#include"soccerPitch.h"




class clsSteeringBehavior{
    private:

        clsSoccerBall* ball;
        clsPlayerBase* player;
        clsSoccerPitch* pitch;
        
    public:
        clsSteeringBehavior(clsPlayerBase* player);

        //seek function generate the required force to reach the tartgetPos
        clsVector2d seek(clsVector2d targetPos);

        enum Deceleration{slow = 3, normal = 2, fast = 1};
        clsVector2d arrive(clsVector2d targetPos, Deceleration deceleration);

        const double MAXSTEERINGFORCE = 100;
        clsVector2d calculate();

        clsVector2d pursuit(clsMovingEntity* evader);

};