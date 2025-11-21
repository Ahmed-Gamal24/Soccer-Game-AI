#pragma once 

#include"fieldPlayer.h"
#include"goalKeeper.h"

class clsSoccerTeam{

private:


public:
    void update();//update to the new state
    void render();//draw visual representation of current state

    bool canShoot(clsPlayerBase from ,clsPlayerBase to,clsPlayerBase heading, int power);
    void requestPass(clsPlayerBase* );

    clsFieldPlayer player1;
    clsFieldPlayer player2;
    clsFieldPlayer player3;
    clsFieldPlayer player4;
    clsGoalKeeper goalKeeper;


};