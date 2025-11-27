#pragma once 

#include"playerBase.h"
#include"fieldPlayer.h"
#include"goalKeeper.h"


struct SupportSpot
{
    clsVector2d pos;
    double score;//we should choose the supporting spot with high score
    SupportSpot(clsVector2d pos, double val):pos(pos),
    score(val){}
};

class clsSoccerTeam{

private:


public:
    void update();//update to the new state
    void render();//draw visual representation of current state

    bool canShoot(clsPlayerBase from ,clsPlayerBase to,clsPlayerBase heading, int power);
    void requestPass(clsPlayerBase* );

    clsPlayerBase* recevingPlayer;
    clsPlayerBase* playerClosestToBall;
    clsPlayerBase* controllingPlayer;
    clsPlayerBase* supportingPlayer;


};