#pragma once 

#include<vector>

#include"playerBase.h"
#include"fieldPlayer.h"
#include"goalKeeper.h"
#include"stateMachine.h"


// Team color enum
enum class TeamColor {
    Red,
    Blue,
    Green
};

struct SupportSpot
{
    clsVector2d pos;
    double score;//we should choose the supporting spot with high score
    SupportSpot(clsVector2d pos, double val):pos(pos),
    score(val){}
};


class clsSupportSpotCalculator{
    private:
        clsSoccerTeam* team;

        //used criaterias to give score to each spot 
        double canPassSafeStrength= 2.0;
        double canScoreFromPosition = 1.0;
        double distanceFromControllingPlayer; //200px

        std::vector<SupportSpot> spots;

    public:
        double optimalDistance = 200;
        clsSupportSpotCalculator(clsSoccerTeam* team);

        clsVector2d determineBestSpot();
        
};


class clsSoccerTeam{

private:

    clsPlayerBase* recevingPlayer;
    clsPlayerBase* playerClosestToBall;
    clsPlayerBase* controllingPlayer;
    clsPlayerBase* supportingPlayer;
    TeamColor teamColor;
    clsStateMachine<clsSoccerTeam> stateMachine;

public:
    clsSoccerTeam() : recevingPlayer(nullptr), playerClosestToBall(nullptr), 
                      controllingPlayer(nullptr), supportingPlayer(nullptr), 
                      teamColor(TeamColor::Red) {}
    
    void setColor(TeamColor color);
    void update();//update to the new state
    void render();//draw visual representation of current state

    bool canShoot(clsPlayerBase from ,clsPlayerBase to,clsPlayerBase heading, int power);
    void requestPass(clsPlayerBase* );
    bool isPassSafeFromOpponents();

    void setReceivingPlayer(clsPlayerBase* player);
    void setPlayerClosestToBall(clsPlayerBase* player);
    void setControllingPlayer(clsPlayerBase* player);
    void setSupportingPlayer(clsPlayerBase* player);

    clsPlayerBase* getReceivingPlayer();
    clsPlayerBase* getPlayerClosestToBall();
    clsPlayerBase* getControllingPlayer();
    clsPlayerBase* getSupportingPlayer();

    void determineBestSupportingSpot(clsSupportSpotCalculator* supportSpotCalculator);




};


