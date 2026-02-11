// Include base classes first to ensure full definitions are available
#include "../include/baseGameEntity.h"
#include "../include/movingEntity.h"
#include "../include/playerBase.h"
#include "../include/fieldPlayer.h"
#include "../include/goalKeeper.h"
#include "../include/soccerBall.h"
#include "../include/goal.h"
#include "../include/soccerTeam.h"
#include "../include/teamStates.h"
// Include main header last (it uses forward declarations, but we need full defs above)
#include "../include/soccerPitch.h"

clsSoccerPitch::clsSoccerPitch(SDL_Renderer *renderer, int w, int h)
{
    _renderer = renderer;
    _width = w;
    _height = h;
    _gameOn = true;

    // Initialize scoring
    redScore = 0;
    blueScore = 0;

    // creating the two goals for two teams
    clsVector2d redTeamLeftPost(_width / 8, _height / 3);
    clsVector2d redTeamRightPost(_width / 8, (_height * 2) / 3);
    redGoal = new clsGoal(redTeamLeftPost, redTeamRightPost);

    clsVector2d blueTeamLeftPost((_width * 7) / 8, (_height * 2) / 3);
    clsVector2d blueTeamRightPost((_width * 7) / 8, _height / 3);
    blueGoal = new clsGoal(blueTeamLeftPost, blueTeamRightPost);

    // the 4 determining game space
    clsVector2d topLeftPoint(0, 0);
    clsVector2d topRightPoint(_width, 0);
    clsVector2d bottomLeftPoint(0, _height);
    clsVector2d bottomRightPoint(_width, _height);

    // game space has 4 walls
    boundries = new clsWall[4];
    // top wall
    boundries[0].setWall(topLeftPoint, topRightPoint);
    // bottom wall
    boundries[1].setWall(bottomLeftPoint, bottomRightPoint);
    // right wall
    boundries[2].setWall(topRightPoint, bottomRightPoint);
    // left wall
    boundries[3].setWall(topLeftPoint, bottomLeftPoint);

    // start the soccerBall at the center of stadium
    clsVector2d ballPos(_width / 2, _height / 2);
    ball = new clsSoccerBall(ballPos, 10, 10, 1, boundries);

    // Kick the ball toward the blue goal to trigger gameplay
    clsVector2d kickDirection(1, 0); // Toward right (blue goal)
    ball->kick(kickDirection, 300);  // Stronger kick

    // Initialize teams
    redTeam = new clsSoccerTeam();
    redTeam->setColor(TeamColor::Red);
    redTeam->stateMachine.setCurrentState(new PrepareForKickOff());

    blueTeam = new clsSoccerTeam();
    blueTeam->setColor(TeamColor::Blue);
    blueTeam->stateMachine.setCurrentState(new PrepareForKickOff());

    // Create players for red team
    // Create controlling player
    clsFieldPlayer *redControlling = new clsFieldPlayer();
    redControlling->setColor(255, 0, 0);
    redControlling->position = clsVector2d(_width / 4, _height / 2);
    redControlling->soccerPitch = this;
    redTeam->setControllingPlayer(redControlling);
    redTeam->addFieldPlayer(redControlling);

    // Create player closest to ball
    clsFieldPlayer *redClosestToBall = new clsFieldPlayer();
    redClosestToBall->setColor(255, 0, 0);
    redClosestToBall->position = clsVector2d(_width / 3, _height / 2);
    redClosestToBall->soccerPitch = this;
    redTeam->setPlayerClosestToBall(redClosestToBall);
    redTeam->addFieldPlayer(redClosestToBall);

    // Create receiving player
    clsFieldPlayer *redReceiving = new clsFieldPlayer();
    redReceiving->setColor(255, 0, 0);
    redReceiving->position = clsVector2d(_width / 3, _height / 3);
    redReceiving->soccerPitch = this;
    redTeam->setReceivingPlayer(redReceiving);
    redTeam->addFieldPlayer(redReceiving);

    // Create supporting player
    clsFieldPlayer *redSupporting = new clsFieldPlayer();
    redSupporting->setColor(255, 0, 0);
    redSupporting->position = clsVector2d(_width / 4, _height * 2 / 3);
    redSupporting->soccerPitch = this;
    redTeam->setSupportingPlayer(redSupporting);
    redTeam->addFieldPlayer(redSupporting);

    // Create goalkeeper
    clsGoalKeeper *redGK = new clsGoalKeeper();
    redGK->setColor(255, 0, 0);
    redGK->position = clsVector2d(_width / 8, _height / 2);
    redGK->soccerPitch = this;
    redTeam->setGoalKeeper(redGK);

    // Create players for blue team
    // Create controlling player
    clsFieldPlayer *blueControlling = new clsFieldPlayer();
    blueControlling->setColor(0, 0, 255);
    blueControlling->position = clsVector2d(_width * 3 / 4, _height / 2);
    blueControlling->soccerPitch = this;
    blueTeam->setControllingPlayer(blueControlling);
    blueTeam->addFieldPlayer(blueControlling);

    // Create player closest to ball
    clsFieldPlayer *blueClosestToBall = new clsFieldPlayer();
    blueClosestToBall->setColor(0, 0, 255);
    blueClosestToBall->position = clsVector2d(_width * 2 / 3, _height / 2);
    blueClosestToBall->soccerPitch = this;
    blueTeam->setPlayerClosestToBall(blueClosestToBall);
    blueTeam->addFieldPlayer(blueClosestToBall);

    // Create receiving player
    clsFieldPlayer *blueReceiving = new clsFieldPlayer();
    blueReceiving->setColor(0, 0, 255);
    blueReceiving->position = clsVector2d(_width * 2 / 3, _height * 2 / 3);
    blueReceiving->soccerPitch = this;
    blueTeam->setReceivingPlayer(blueReceiving);
    blueTeam->addFieldPlayer(blueReceiving);

    // Create supporting player
    clsFieldPlayer *blueSupporting = new clsFieldPlayer();
    blueSupporting->setColor(0, 0, 255);
    blueSupporting->position = clsVector2d(_width * 3 / 4, _height / 3);
    blueSupporting->soccerPitch = this;
    blueTeam->setSupportingPlayer(blueSupporting);
    blueTeam->addFieldPlayer(blueSupporting);

    // Create goalkeeper
    clsGoalKeeper *blueGK = new clsGoalKeeper();
    blueGK->setColor(0, 0, 255);
    blueGK->position = clsVector2d(_width * 7 / 8, _height / 2);
    blueGK->soccerPitch = this;
    blueTeam->setGoalKeeper(blueGK);

    // set 24 regions
    const int cols = 8;
    const int rows = 3;
    const int numOfRegions = cols * rows;

    regions.resize(numOfRegions);

    double regionWidth = _width / static_cast<double>(cols);
    double regionHeight = _height / static_cast<double>(rows);

    int index = 0;
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            // top left point
            double x0 = c * regionWidth;
            double y0 = r * regionHeight;
            // bottom right point
            double x1 = x0 + regionWidth;
            double y1 = y0 + regionHeight;

            regions[index].setRegPos(clsVector2d(x0, y0), clsVector2d(x1, y1));
            index++;
        }
    }

    // set home regions of players in two teams
    redTeam->getControllingPlayer()->setPlayerHomeRegion(regions[5].getCenter());
    redTeam->getPlayerClosestToBall()->setPlayerHomeRegion(regions[13].getCenter());
    redTeam->getReceivingPlayer()->setPlayerHomeRegion(regions[14].getCenter());
    redTeam->getSupportingPlayer()->setPlayerHomeRegion(regions[20].getCenter());
    redTeam->getGoalKeeper()->setPlayerHomeRegion(regions[15].getCenter());

    blueTeam->getControllingPlayer()->setPlayerHomeRegion(regions[2].getCenter());
    blueTeam->getPlayerClosestToBall()->setPlayerHomeRegion(regions[11].getCenter());
    blueTeam->getReceivingPlayer()->setPlayerHomeRegion(regions[9].getCenter());
    blueTeam->getSupportingPlayer()->setPlayerHomeRegion(regions[18].getCenter());
    blueTeam->getGoalKeeper()->setPlayerHomeRegion(regions[8].getCenter());
}

double clsSoccerPitch::getWidth()
{
    return _width;
}
double clsSoccerPitch::getHeight()
{
    return _height;
}
clsSoccerBall *clsSoccerPitch::getBall()
{
    return ball;
}
void clsSoccerPitch::update()
{
    // Update ball
    if (ball != nullptr)
    {
        ball->update(1.0 / 60.0); // Assuming 60 FPS
    }

    // Update teams (their state machines)
    if (redTeam != nullptr)
    {
        redTeam->update();
    }
    if (blueTeam != nullptr)
    {
        blueTeam->update();
    }

    // Update all field players
    if (redTeam != nullptr)
    {
        for (auto *player : redTeam->getFieldPlayers())
        {
            if (player != nullptr)
            {
                player->update();
            }
        }
        // Update goalkeeper
        if (redTeam->getGoalKeeper() != nullptr)
        {
            redTeam->getGoalKeeper()->update();
        }
    }
    if (blueTeam != nullptr)
    {
        for (auto *player : blueTeam->getFieldPlayers())
        {
            if (player != nullptr)
            {
                player->update();
            }
        }
        // Update goalkeeper
        if (blueTeam->getGoalKeeper() != nullptr)
        {
            blueTeam->getGoalKeeper()->update();
        }
    }

    // Check for goals
    if (ball != nullptr && _gameOn)
    {
        if (redGoal != nullptr && redGoal->isScored(ball))
        {
            // Goal scored on red team - blue team scores
            blueScore++;
            std::cout << "\n=== GOAL! Blue Team Scores! ===" << std::endl;
            std::cout << "Score: Red " << redScore << " - Blue " << blueScore << std::endl;

            // Check for win condition
            if (blueScore >= WINNING_SCORE)
            {
                std::cout << "\n!!! BLUE TEAM WINS THE GAME !!!" << std::endl;
                _gameOn = false;
            }
            else
            {
                resetGame();
            }
        }
        else if (blueGoal != nullptr && blueGoal->isScored(ball))
        {
            // Goal scored on blue team - red team scores
            redScore++;
            std::cout << "\n=== GOAL! Red Team Scores! ===" << std::endl;
            std::cout << "Score: Red " << redScore << " - Blue " << blueScore << std::endl;

            // Check for win condition
            if (redScore >= WINNING_SCORE)
            {
                std::cout << "\n!!! RED TEAM WINS THE GAME !!!" << std::endl;
                _gameOn = false;
            }
            else
            {
                resetGame();
            }
        }
    }
}

void clsSoccerPitch::resetGame()
{
}

void clsSoccerPitch::render()
{
    // Clear screen with white background
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);

    // Render all game entities
    // render center line
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Black color
    SDL_RenderDrawLine(_renderer, static_cast<int>(_width / 2), 0, static_cast<int>(_width / 2), static_cast<int>(_height));

    // Render goals
    if (redGoal != nullptr)
    {
        redGoal->render(_renderer);
    }
    if (blueGoal != nullptr)
    {
        blueGoal->render(_renderer);
    }

    // Render soccer ball
    if (ball != nullptr)
    {
        ball->render(_renderer, 0, 0, 0); // Black ball
    }

    // Render red team players
    if (redTeam != nullptr)
    {
        for (auto *player : redTeam->getFieldPlayers())
        {
            if (player != nullptr)
            {
                player->render(_renderer);
            }
        }
        // Render red goalkeeper
        if (redTeam->getGoalKeeper() != nullptr)
        {
            redTeam->getGoalKeeper()->render(_renderer);
        }
    }

    // Render blue team players
    if (blueTeam != nullptr)
    {
        for (auto *player : blueTeam->getFieldPlayers())
        {
            if (player != nullptr)
            {
                player->render(_renderer);
            }
        }
        // Render blue goalkeeper
        if (blueTeam->getGoalKeeper() != nullptr)
        {
            blueTeam->getGoalKeeper()->render(_renderer);
        }
    }

    SDL_RenderPresent(_renderer);
}
