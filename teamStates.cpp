#include"teamStates.h"
#include"soccerTeam.h"
#include"telegram.h"
#include"playerBase.h"
#include"soccerPitch.h"

void Attacking::Enter(clsSoccerTeam* entity){
    entity->setColor(TeamColor::Red);
}

void Attacking::Execute(clsSoccerTeam* entity){
    // In attacking state, team tries to score
    // Could update player positions, assign roles, etc.
    entity->setColor(TeamColor::Red);
}

void Attacking::Exit(clsSoccerTeam* entity){
    entity->setColor(TeamColor::Red);
}

bool Attacking::OnMessage(clsSoccerTeam* entity, const clsTelegram& msg){
    // Handle messages specific to attacking state
    return false;
}

void Defending::Enter(clsSoccerTeam* entity){
    entity->setColor(TeamColor::Blue);
}

void Defending::Execute(clsSoccerTeam* entity){
    // In defending state, team protects goal
    // Could assign defensive positions, mark opponents, etc.
    entity->setColor(TeamColor::Blue);
}

void Defending::Exit(clsSoccerTeam* entity){
    entity->setColor(TeamColor::Blue);
}

bool Defending::OnMessage(clsSoccerTeam* entity, const clsTelegram& msg){
    // Handle messages specific to defending state
    return false;
}

void PrepareForKickOff::Enter(clsSoccerTeam* entity){
    entity->setColor(TeamColor::Green);
}

void PrepareForKickOff::Execute(clsSoccerTeam* entity){
    // Prepare for kickoff - position players at starting positions
    entity->setColor(TeamColor::Green);
}

void PrepareForKickOff::Exit(clsSoccerTeam* entity){
    entity->setColor(TeamColor::Green);
}

bool PrepareForKickOff::OnMessage(clsSoccerTeam* entity, const clsTelegram& msg){
    // Handle messages for kickoff preparation
    return false;
}
