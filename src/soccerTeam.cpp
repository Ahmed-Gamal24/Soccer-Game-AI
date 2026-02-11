#include "../include/soccerTeam.h"
#include "../include/playerBase.h"
#include "../include/fieldPlayer.h"
#include "../include/goalKeeper.h"
#include "../include/teamStates.h"

//============= implementation of clsSupportSpotCalculator class======================

clsSupportSpotCalculator::clsSupportSpotCalculator(clsSoccerTeam *team)
{
    this->team = team;
}

clsVector2d clsSupportSpotCalculator::determineBestSpot()
{

    double bestScore = 0;
    SupportSpot *bestSupportingSpot = nullptr;

    std::vector<SupportSpot>::iterator currentSpot;
    // visit every spot
    // determine it's score based on some tests
    // update bestScore variable and bestSupportingSpot pointer
    for (currentSpot = spots.begin(); currentSpot != spots.end(); ++currentSpot)
    {

        currentSpot->score = 1.0;

        // test canPassSafe
        if (team->isPassSafeFromOpponents())
        {
            currentSpot->score += canPassSafeStrength;
        }

        // test canScoreFromPosition
        // Note: canShoot requires parameters, this is a placeholder
        // TODO: Implement proper shooting check with actual parameters
        if (team->getControllingPlayer() != nullptr)
        {
            // Placeholder - should check if can shoot from this position
            // currentSpot->score += canScoreFromPosition;
        }

        // test distanceFromControllingPlayer
        // if (team->getControllingPlayer() != nullptr) {
        //     double distance = team->getControllingPlayer()->position.distance(currentSpot->pos);
        //     if (distance < optimalDistance)
        //     {
        //         currentSpot->score += distanceFromControllingPlayer;
        //     }
        // }

        if (currentSpot->score > bestScore)
        {
            bestScore = currentSpot->score;
            bestSupportingSpot = &(*currentSpot);
        }
    }

    // Return best spot position, or default position if no spots found
    if (bestSupportingSpot != nullptr)
    {
        return bestSupportingSpot->pos;
    }

    // Default: return center of field if no spots available
    // This should not happen in normal gameplay, but prevents crash
    return clsVector2d(0, 0);
}

//============= implementation of clsSoccerTeam class======================

clsSoccerTeam::clsSoccerTeam() : recevingPlayer(nullptr), playerClosestToBall(nullptr),
                                 controllingPlayer(nullptr), supportingPlayer(nullptr),
                                 goalKeeper(nullptr),
                                 teamColor(TeamColor::Red),
                                 stateMachine(this)
{
    stateMachine.setCurrentState(new PrepareForKickOff());
}

void clsSoccerTeam::update()
{
    stateMachine.updateStateMachine();
}

void clsSoccerTeam::render()
{
    // Render the team - visualization stub
}

bool clsSoccerTeam::canShoot(clsPlayerBase from, clsPlayerBase to, clsPlayerBase heading, int power)
{
    // Decide if from can shoot at to, in heading direction, with given power
    // For now, left as stub - should check for shooting range, clear line, etc.
    return false;
}

void clsSoccerTeam::requestPass(clsPlayerBase *player)
{
    // Logic for a player to request a pass from controlling player
    // For now, left as stub
}

bool clsSoccerTeam::isPassSafeFromOpponents()
{
    // Determine if current pass is safe from being intercepted by opponents
    // For now, left as stub
    return true;
}

void clsSoccerTeam::setReceivingPlayer(clsPlayerBase *player)
{
    recevingPlayer = player;
}

void clsSoccerTeam::setPlayerClosestToBall(clsPlayerBase *player)
{
    playerClosestToBall = player;
}

void clsSoccerTeam::setControllingPlayer(clsPlayerBase *player)
{
    controllingPlayer = player;
}

void clsSoccerTeam::setSupportingPlayer(clsPlayerBase *player)
{
    supportingPlayer = player;
}

void clsSoccerTeam::setGoalKeeper(clsGoalKeeper *player)
{
    goalKeeper = player;
}

clsPlayerBase *clsSoccerTeam::getReceivingPlayer()
{
    return recevingPlayer;
}

clsPlayerBase *clsSoccerTeam::getPlayerClosestToBall()
{
    return playerClosestToBall;
}

clsPlayerBase *clsSoccerTeam::getControllingPlayer()
{
    return controllingPlayer;
}

clsPlayerBase *clsSoccerTeam::getSupportingPlayer()
{
    return supportingPlayer;
}

clsGoalKeeper *clsSoccerTeam::getGoalKeeper()
{
    return goalKeeper;
}

void clsSoccerTeam::determineBestSupportingSpot(clsSupportSpotCalculator *supportSpotCalculator)
{
    // This would use the supportSpotCalculator to find the best spot and assign supportingPlayer to it, or similar logic
    // For now, left as stub
}

void clsSoccerTeam::setColor(TeamColor color)
{
    teamColor = color;

    // Set color for all field players based on team color
    uint8_t r, g, b;
    if (color == TeamColor::Red)
    {
        r = 255;
        g = 0;
        b = 0; // Red
    }
    else if (color == TeamColor::Blue)
    {
        r = 0;
        g = 0;
        b = 255; // Blue
    }
    else if (color == TeamColor::Green)
    {
        r = 0;
        g = 255;
        b = 0; // Green
    }

    for (auto *player : fieldPlayers)
    {
        if (player != nullptr)
        {
            player->setColor(r, g, b);
        }
    }

    // Also set goalkeeper color
    if (goalKeeper != nullptr)
    {
        goalKeeper->setColor(r, g, b);
    }
}

void clsSoccerTeam::returnAllFieldPlayersToHome()
{
    // Send Msg_GoHome to all field players
    for (auto *player : fieldPlayers)
    {
        if (player != nullptr)
        {
            clsTelegram msg(-1, player->getId(), Msg_GoHome, 0.0);
            player->handleMessage(msg);
        }
    }
}

void clsSoccerTeam::addFieldPlayer(clsFieldPlayer *player)
{
    if (player != nullptr)
    {
        fieldPlayers.push_back(player);
    }
}

std::vector<clsFieldPlayer *> &clsSoccerTeam::getFieldPlayers()
{
    return fieldPlayers;
}

bool clsSoccerTeam::hasTheBall()
{
    // Check if any player on this team owns the ball
    for (auto player : fieldPlayers)
    {
        if (player != nullptr && controllingPlayer == player)
        {
            return true;
        }
    }

    // Check if goalkeeper has ball
    if (goalKeeper != nullptr && controllingPlayer == goalKeeper)
    {
        return true;
    }

    return false;
}

TeamColor clsSoccerTeam::getTeamColor()
{
    return teamColor;
}
