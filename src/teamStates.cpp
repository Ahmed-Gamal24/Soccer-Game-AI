#include "../include/teamStates.h"
// Include full definitions for implementation
#include "../include/soccerTeam.h"
#include "../include/telegram.h"
#include "../include/playerBase.h"
#include "../include/fieldPlayer.h"
#include "../include/soccerPitch.h"

void Attacking::Enter(clsSoccerTeam *entity)
{
    // Team starts attacking - designate players for roles
    // Controlling player leads attack
    // Other players take supporting roles
}

void Attacking::Execute(clsSoccerTeam *entity)
{
    // Check if team still has ball possession
    // If we lost the ball, transition to Defending
    if (!entity->hasTheBall())
    {
        entity->stateMachine.changeState(new Defending());
    }
}

void Attacking::Exit(clsSoccerTeam *entity)
{
    // Team stops attacking
}

bool Attacking::OnMessage(clsSoccerTeam *entity, const clsTelegram &msg)
{
    // Handle messages specific to attacking state
    switch (msg.msg)
    {
    case Msg_GoHome:
        // If told to return home (game reset), go to PrepareForKickOff
        entity->stateMachine.changeState(new PrepareForKickOff());
        return true;
    default:
        return false;
    }
}

void Defending::Enter(clsSoccerTeam *entity)
{
    // Team starts defending - position players to block shots
    // Send players toward own goal to defend
}

void Defending::Execute(clsSoccerTeam *entity)
{
    // Check if we regain possession
    // If we get the ball back, transition to Attacking
    if (entity->hasTheBall())
    {
        entity->stateMachine.changeState(new Attacking());
    }
}

void Defending::Exit(clsSoccerTeam *entity)
{
    // Team stops defending
}

bool Defending::OnMessage(clsSoccerTeam *entity, const clsTelegram &msg)
{
    // Handle messages specific to defending state
    switch (msg.msg)
    {
    case Msg_GoHome:
        // If told to return home (game reset), go to PrepareForKickOff
        entity->stateMachine.changeState(new PrepareForKickOff());
        return true;
    default:
        return false;
    }
}

void PrepareForKickOff::Enter(clsSoccerTeam *entity)
{
    // Team prepares for kickoff - reset positions
    entity->setControllingPlayer(nullptr);
    entity->setPlayerClosestToBall(nullptr);
    entity->setReceivingPlayer(nullptr);
    entity->setSupportingPlayer(nullptr);

    // Send Msg_GoHome to each player to return to home regions
    entity->returnAllFieldPlayersToHome();
}

void PrepareForKickOff::Execute(clsSoccerTeam *entity)
{
    // Check if all players are ready (reached home positions)
    bool allPlayersReady = true;
    for (auto player : entity->getFieldPlayers())
    {
        if (player != nullptr)
        {
            clsVector2d homeRegion = player->getPlayerHomeRegion();
            double distanceToHome = player->position.distance(homeRegion);
            if (distanceToHome > 100.0) // Not at home
            {
                allPlayersReady = false;
                break;
            }
        }
    }

    // Once all players are ready, start the game
    if (allPlayersReady)
    {
        // Start with one deterministic kickoff side.
        if (entity->getTeamColor() == TeamColor::Red)
        {
            entity->stateMachine.changeState(new Attacking());
        }
        else
        {
            entity->stateMachine.changeState(new Defending());
        }
    }
}

void PrepareForKickOff::Exit(clsSoccerTeam *entity)
{
    // Game starting
}

bool PrepareForKickOff::OnMessage(clsSoccerTeam *entity, const clsTelegram &msg)
{
    // Handle messages for kickoff preparation
    switch (msg.msg)
    {
    case Msg_GoHome:
        // Already in prep mode, acknowledge
        return true;
    default:
        return false;
    }
}
