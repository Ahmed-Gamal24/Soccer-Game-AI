// Include base classes first to ensure full definitions are available
#include "baseGameEntity.h"
#include "movingEntity.h"
#include "playerBase.h"
#include "fieldPlayer.h"
#include "goalKeeper.h"
#include "soccerBall.h"
#include "goal.h"
#include "soccerTeam.h"
#include "teamStates.h"
// Include main header last (it uses forward declarations, but we need full defs above)
#include "soccerPitch.h"

clsSoccerPitch::clsSoccerPitch()
{

    _width = 2500;
    _height = 1000;
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
    redControlling->position = clsVector2d(_width / 4, _height / 2);
    redControlling->soccerPitch = this;
    redTeam->setControllingPlayer(redControlling);
    redTeam->addFieldPlayer(redControlling);

    // Create player closest to ball
    clsFieldPlayer *redClosestToBall = new clsFieldPlayer();
    redClosestToBall->position = clsVector2d(_width / 3, _height / 2);
    redClosestToBall->soccerPitch = this;
    redTeam->setPlayerClosestToBall(redClosestToBall);
    redTeam->addFieldPlayer(redClosestToBall);

    // Create receiving player
    clsFieldPlayer *redReceiving = new clsFieldPlayer();
    redReceiving->position = clsVector2d(_width / 3, _height / 3);
    redReceiving->soccerPitch = this;
    redTeam->setReceivingPlayer(redReceiving);
    redTeam->addFieldPlayer(redReceiving);

    // Create supporting player
    clsFieldPlayer *redSupporting = new clsFieldPlayer();
    redSupporting->position = clsVector2d(_width / 4, _height * 2 / 3);
    redSupporting->soccerPitch = this;
    redTeam->setSupportingPlayer(redSupporting);
    redTeam->addFieldPlayer(redSupporting);

    // Create goalkeeper
    clsGoalKeeper *redGK = new clsGoalKeeper();
    redGK->position = clsVector2d(_width / 8, _height / 2);
    redGK->soccerPitch = this;
    redTeam->setGoalKeeper(redGK);

    // Create players for blue team
    // Create controlling player
    clsFieldPlayer *blueControlling = new clsFieldPlayer();
    blueControlling->position = clsVector2d(_width * 3 / 4, _height / 2);
    blueControlling->soccerPitch = this;
    blueTeam->setControllingPlayer(blueControlling);
    blueTeam->addFieldPlayer(blueControlling);

    // Create player closest to ball
    clsFieldPlayer *blueClosestToBall = new clsFieldPlayer();
    blueClosestToBall->position = clsVector2d(_width * 2 / 3, _height / 2);
    blueClosestToBall->soccerPitch = this;
    blueTeam->setPlayerClosestToBall(blueClosestToBall);
    blueTeam->addFieldPlayer(blueClosestToBall);

    // Create receiving player
    clsFieldPlayer *blueReceiving = new clsFieldPlayer();
    blueReceiving->position = clsVector2d(_width * 2 / 3, _height * 2 / 3);
    blueReceiving->soccerPitch = this;
    blueTeam->setReceivingPlayer(blueReceiving);
    blueTeam->addFieldPlayer(blueReceiving);

    // Create supporting player
    clsFieldPlayer *blueSupporting = new clsFieldPlayer();
    blueSupporting->position = clsVector2d(_width * 3 / 4, _height / 3);
    blueSupporting->soccerPitch = this;
    blueTeam->setSupportingPlayer(blueSupporting);
    blueTeam->addFieldPlayer(blueSupporting);

    // Create goalkeeper
    clsGoalKeeper *blueGK = new clsGoalKeeper();
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
    // Reset ball to center
    clsVector2d ballPos(_width / 2, _height / 2);
    ball->placeBallAtPosition(ballPos);
    ball->velocity = clsVector2d(0, 0);
    ball->setBallOwner(nullptr);

    // Reset all players to home positions
    if (redTeam != nullptr)
    {
        redTeam->returnAllFieldPlayersToHome();
        redTeam->stateMachine.changeState(new PrepareForKickOff());
    }
    if (blueTeam != nullptr)
    {
        blueTeam->returnAllFieldPlayersToHome();
        blueTeam->stateMachine.changeState(new PrepareForKickOff());
    }

    std::cout << "Game reset! Preparing for next round..." << std::endl;
}

void clsSoccerPitch::render()
{
    // --- 1. Initialization ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        // cerr is used for error messages
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    // --- 2. Window and Renderer Creation ---
    // This creates both the window and its associated drawing context (renderer).
    if (SDL_CreateWindowAndRenderer(
            "Soccer AI Game", // Window title
            _width,           // Width
            _height,          // Height
            0,                // Flags: 0 -> unresiable window
            &window,          // Pointer to the created window
            &renderer         // Pointer to the created renderer
            ) < 0)
    {
        std::cerr << "Window/Renderer creation failed! SDL Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    // --- 3. The Main Game Loop ---
    bool IsRunning = true;
    SDL_Event Event;

    while (IsRunning)
    {

        // --- A. Event Polling (Check for user input) ---
        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_EVENT_QUIT)
            {
                IsRunning = false;
            }
        }

        // --- B. Rendering Steps (Drawing logic goes here) ---

        // Clear the screen to a background color (white)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Set color for game elements (black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

        // draw 4 walls which determine the game space
        for (int i = 0; i < 4; ++i)
        {
            boundries[i].render(renderer);
        }

        // Draw the center Line
        SDL_RenderLine(renderer, _width / 2, 0, _width / 2, _height);

        // draw the two goals
        redGoal->render(renderer);
        blueGoal->render(renderer);

        // Update game state
        update();

        // draw the teams players
        if (redTeam != nullptr)
        {
            for (auto *player : redTeam->getFieldPlayers())
            {
                if (player != nullptr)
                {
                    player->render(renderer);
                }
            }
            if (redTeam->getGoalKeeper() != nullptr)
            {
                redTeam->getGoalKeeper()->render(renderer);
            }
        }

        if (blueTeam != nullptr)
        {
            for (auto *player : blueTeam->getFieldPlayers())
            {
                if (player != nullptr)
                {
                    player->render(renderer);
                }
            }
            if (blueTeam->getGoalKeeper() != nullptr)
            {
                blueTeam->getGoalKeeper()->render(renderer);
            }
        }

        // draw the ball (uses its own position and radius)
        if (ball != nullptr)
        {
            ball->render(renderer, 0, 0, 0); // Black ball
        }

        // Draw score display in console
        // (SDL3 doesn't have built-in text rendering, so we rely on console output)
        if (!_gameOn)
        {
            std::cout << "\n*** GAME OVER ***" << std::endl;
            if (redScore >= WINNING_SCORE)
            {
                std::cout << "RED TEAM WINS! Final Score: " << redScore << " - " << blueScore << std::endl;
            }
            else if (blueScore >= WINNING_SCORE)
            {
                std::cout << "BLUE TEAM WINS! Final Score: " << redScore << " - " << blueScore << std::endl;
            }
        }

        // Update the Screen
        SDL_RenderPresent(renderer);

        // Simple delay to cap frame rate (approx. 60 FPS)
        SDL_Delay(1000 / 60);
    }

    // --- 4. Cleanup ---
    // Destroy the renderer first
    SDL_DestroyRenderer(renderer);
    // Destroy the window
    SDL_DestroyWindow(window);
    // Shut down all SDL subsystems
    SDL_Quit();
}
