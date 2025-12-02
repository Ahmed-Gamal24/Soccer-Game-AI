#include"soccerTeam.h"

//============= implementation of clsSupportSpotCalculator class======================

clsSupportSpotCalculator::clsSupportSpotCalculator(clsSoccerTeam* team){
    this->team = team;
}

clsVector2d clsSupportSpotCalculator::determineBestSpot(){
    
    double bestScore = 0;
    SupportSpot* bestSupportingSpot = nullptr;
    
    std::vector<SupportSpot>::iterator currentSpot;
    //visit every spot 
    //determine it's score based on some tests
    //update bestScore variable and bestSupportingSpot pointer 
    for(currentSpot = spots.begin(); currentSpot != spots.end(); ++currentSpot)
    {
        
        currentSpot->score = 1.0;
        
        //test canPassSafe
        if(team->isPassSafeFromOpponents()){
            currentSpot->score += canPassSafeStrength;
        }
        
        //test canScoreFromPosition
        if(team->canShoot()){
            currentSpot->score += canScoreFromPosition;
        }
        
        //test distanceFromControllingPlayer
        double distance = team->getControllingPlayer()->position.distance(currentSpot->pos);
        if(distance < optimalDistance){
            currentSpot->score += distanceFromControllingPlayer;
        }

        if(currentSpot->score > bestScore){
            bestScore = currentSpot->score;
            bestSupportingSpot = &(*currentSpot);
        }
    }

    return bestSupportingSpot->pos;
    
}





//============= implementation of clsSoccerTeam class======================

clsSoccerTeam::clsSoccerTeam() : teamColor(TeamColor::Red) {
    stateMachine.setCurrentState(new Attacking());
}

void clsSoccerTeam::update() {
    stateMachine.update();
}

void clsSoccerTeam::render() {
    // Render the team - visualization stub
}

bool clsSoccerTeam::canShoot(clsPlayerBase from, clsPlayerBase to, clsPlayerBase heading, int power) {
    // Decide if from can shoot at to, in heading direction, with given power
    // For now, left as stub - should check for shooting range, clear line, etc.
    return false;
}

void clsSoccerTeam::requestPass(clsPlayerBase* player) {
    // Logic for a player to request a pass from controlling player
    // For now, left as stub
}

bool clsSoccerTeam::isPassSafeFromOpponents() {
    // Determine if current pass is safe from being intercepted by opponents
    // For now, left as stub
    return true;
}

void clsSoccerTeam::setReceivingPlayer(clsPlayerBase* player) {
    recevingPlayer = player;
}

void clsSoccerTeam::setPlayerClosestToBall(clsPlayerBase* player) {
    playerClosestToBall = player;
}

void clsSoccerTeam::setControllingPlayer(clsPlayerBase* player) {
    controllingPlayer = player;
}

void clsSoccerTeam::setSupportingPlayer(clsPlayerBase* player) {
    supportingPlayer = player;
}

clsPlayerBase* clsSoccerTeam::getReceivingPlayer() {
    return recevingPlayer;
}

clsPlayerBase* clsSoccerTeam::getPlayerClosestToBall() {
    return playerClosestToBall;
}

clsPlayerBase* clsSoccerTeam::getControllingPlayer() {
    return controllingPlayer;
}

clsPlayerBase* clsSoccerTeam::getSupportingPlayer() {
    return supportingPlayer;
}

void clsSoccerTeam::determineBestSupportingSpot(clsSupportSpotCalculator* supportSpotCalculator) {
    // This would use the supportSpotCalculator to find the best spot and assign supportingPlayer to it, or similar logic
    // For now, left as stub
}

void clsSoccerTeam::setColor(TeamColor color) {
    teamColor = color;
}





