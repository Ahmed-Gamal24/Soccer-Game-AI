#pragma once

#include <vector>
#include "vector2d.h"
#include "stateMachine.h"

// Forward declarations
class clsPlayerBase;
class clsFieldPlayer;
class clsGoalKeeper;
class clsSoccerTeam;

// Team color enum
enum class TeamColor
{
    Red,
    Blue,
    Green
};

struct SupportSpot
{
    clsVector2d pos;
    double score; // we should choose the supporting spot with high score
    SupportSpot(clsVector2d pos, double val) : pos(pos),
                                               score(val) {}
};

class clsSupportSpotCalculator
{
private:
    clsSoccerTeam *team;

    // used criaterias to give score to each spot
    double canPassSafeStrength = 2.0;
    double canScoreFromPosition = 1.0;
    double distanceFromControllingPlayer; // 200px

    std::vector<SupportSpot> spots;

public:
    double optimalDistance = 200;
    clsSupportSpotCalculator(clsSoccerTeam *team);

    clsVector2d determineBestSpot();
};

class clsSoccerTeam
{

private:
    clsPlayerBase *recevingPlayer;
    clsPlayerBase *playerClosestToBall;
    clsPlayerBase *controllingPlayer;
    clsPlayerBase *supportingPlayer;
    clsGoalKeeper *goalKeeper;
    TeamColor teamColor;

    // Storage for all field players on the team
    std::vector<clsFieldPlayer *> fieldPlayers;

public:
    clsStateMachine<clsSoccerTeam> stateMachine;

    clsSoccerTeam();

    void setColor(TeamColor color);
    void update(); // update to the new state
    void render(); // draw visual representation of current state

    bool canShoot(clsPlayerBase from, clsPlayerBase to, clsPlayerBase heading, int power);
    void requestPass(clsPlayerBase *);
    bool isPassSafeFromOpponents();

    void setReceivingPlayer(clsPlayerBase *player);
    void setPlayerClosestToBall(clsPlayerBase *player);
    void setControllingPlayer(clsPlayerBase *player);
    void setSupportingPlayer(clsPlayerBase *player);
    void setGoalKeeper(clsGoalKeeper *player);

    clsPlayerBase *getReceivingPlayer();
    clsPlayerBase *getPlayerClosestToBall();
    clsPlayerBase *getControllingPlayer();
    clsPlayerBase *getSupportingPlayer();
    clsGoalKeeper *getGoalKeeper();

    void determineBestSupportingSpot(clsSupportSpotCalculator *supportSpotCalculator);

    void returnAllFieldPlayersToHome();

    // Methods to manage field players
    void addFieldPlayer(clsFieldPlayer *player);
    std::vector<clsFieldPlayer *> &getFieldPlayers();

    // Check team possession
    bool hasTheBall();
    TeamColor getTeamColor();
};
