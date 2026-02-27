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
#include <algorithm>
#include <cmath>

namespace
{
struct BodyProxy
{
    clsMovingEntity *entity;
    double radius;
};

void resolveBodyOverlaps(std::vector<BodyProxy> &bodies)
{
    for (size_t i = 0; i < bodies.size(); ++i)
    {
        for (size_t j = i + 1; j < bodies.size(); ++j)
        {
            clsMovingEntity *a = bodies[i].entity;
            clsMovingEntity *b = bodies[j].entity;
            if (a == nullptr || b == nullptr)
                continue;

            clsVector2d delta = b->position;
            delta.operator-=(a->position);
            double dist = delta.length();
            double minDist = bodies[i].radius + bodies[j].radius;

            if (dist < minDist)
            {
                if (dist < 0.0001)
                {
                    delta = clsVector2d(1.0, 0.0);
                    dist = 1.0;
                }
                else
                {
                    delta.operator/=(dist); // normalize
                }

                double overlap = minDist - dist;
                clsVector2d push = delta;
                push.operator*=(overlap * 0.5);

                a->position.operator-=(push);
                b->position.operator+=(push);
            }
        }
    }
}

void drawSevenSegDigit(SDL_Renderer *renderer, int x, int y, int digit, int scale, uint8_t r, uint8_t g, uint8_t b)
{
    static const bool seg[10][7] = {
        {true, true, true, true, true, true, false},   // 0
        {false, true, true, false, false, false, false}, // 1
        {true, true, false, true, true, false, true},  // 2
        {true, true, true, true, false, false, true},  // 3
        {false, true, true, false, false, true, true}, // 4
        {true, false, true, true, false, true, true},  // 5
        {true, false, true, true, true, true, true},   // 6
        {true, true, true, false, false, false, false}, // 7
        {true, true, true, true, true, true, true},    // 8
        {true, true, true, true, false, true, true}    // 9
    };

    if (digit < 0 || digit > 9)
        digit = 0;

    int t = std::max(2, scale); // segment thickness
    int w = 14 * scale;
    int h = 24 * scale;
    int half = h / 2;

    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);

    // a
    if (seg[digit][0]) { SDL_Rect r0 = {x + t, y, w - 2 * t, t}; SDL_RenderFillRect(renderer, &r0); }
    // b
    if (seg[digit][1]) { SDL_Rect r1 = {x + w - t, y + t, t, half - t}; SDL_RenderFillRect(renderer, &r1); }
    // c
    if (seg[digit][2]) { SDL_Rect r2 = {x + w - t, y + half, t, half - t}; SDL_RenderFillRect(renderer, &r2); }
    // d
    if (seg[digit][3]) { SDL_Rect r3 = {x + t, y + h - t, w - 2 * t, t}; SDL_RenderFillRect(renderer, &r3); }
    // e
    if (seg[digit][4]) { SDL_Rect r4 = {x, y + half, t, half - t}; SDL_RenderFillRect(renderer, &r4); }
    // f
    if (seg[digit][5]) { SDL_Rect r5 = {x, y + t, t, half - t}; SDL_RenderFillRect(renderer, &r5); }
    // g
    if (seg[digit][6]) { SDL_Rect r6 = {x + t, y + half - t / 2, w - 2 * t, t}; SDL_RenderFillRect(renderer, &r6); }
}

void drawScoreHud(SDL_Renderer *renderer, int pitchWidth, int redScore, int blueScore, bool gameOver)
{
    const int hudW = 280;
    const int hudH = 70;
    const int hudX = (pitchWidth - hudW) / 2;
    const int hudY = 12;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 130);
    SDL_Rect bg = {hudX, hudY, hudW, hudH};
    SDL_RenderFillRect(renderer, &bg);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &bg);

    // Team color markers.
    SDL_SetRenderDrawColor(renderer, 220, 60, 60, SDL_ALPHA_OPAQUE);
    SDL_Rect redMarker = {hudX + 14, hudY + 18, 24, 34};
    SDL_RenderFillRect(renderer, &redMarker);

    SDL_SetRenderDrawColor(renderer, 60, 90, 230, SDL_ALPHA_OPAQUE);
    SDL_Rect blueMarker = {hudX + hudW - 38, hudY + 18, 24, 34};
    SDL_RenderFillRect(renderer, &blueMarker);

    // Scores.
    drawSevenSegDigit(renderer, hudX + 70, hudY + 18, redScore, 2, 255, 255, 255);
    drawSevenSegDigit(renderer, hudX + 180, hudY + 18, blueScore, 2, 255, 255, 255);

    // Center separator.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect sep = {hudX + (hudW / 2) - 2, hudY + 20, 4, hudH - 40};
    SDL_RenderFillRect(renderer, &sep);

    // Highlight winner when the game is finished.
    if (gameOver)
    {
        bool redWon = redScore > blueScore;
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, SDL_ALPHA_OPAQUE);
        SDL_Rect winRect = redWon ? SDL_Rect{hudX + 8, hudY + 12, 38, 46}
                                  : SDL_Rect{hudX + hudW - 46, hudY + 12, 38, 46};
        SDL_RenderDrawRect(renderer, &winRect);
        SDL_RenderDrawRect(renderer, &winRect);
    }
}
} // namespace

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
    const double goalLineInset = 2.0;
    clsVector2d redTeamLeftPost(goalLineInset, _height / 3);
    clsVector2d redTeamRightPost(goalLineInset, (_height * 2) / 3);
    redGoal = new clsGoal(redTeamLeftPost, redTeamRightPost);

    clsVector2d blueTeamLeftPost(_width - goalLineInset, (_height * 2) / 3);
    clsVector2d blueTeamRightPost(_width - goalLineInset, _height / 3);
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
    redGK->position = clsVector2d(goalLineInset, _height / 2);
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
    blueGK->position = clsVector2d(_width - goalLineInset, _height / 2);
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
    // Red defends left goal, Blue defends right goal.
    redTeam->getControllingPlayer()->setPlayerHomeRegion(regions[2].getCenter());
    redTeam->getPlayerClosestToBall()->setPlayerHomeRegion(regions[11].getCenter());
    redTeam->getReceivingPlayer()->setPlayerHomeRegion(regions[9].getCenter());
    redTeam->getSupportingPlayer()->setPlayerHomeRegion(regions[18].getCenter());
    redTeam->getGoalKeeper()->setPlayerHomeRegion(regions[8].getCenter());

    blueTeam->getControllingPlayer()->setPlayerHomeRegion(regions[5].getCenter());
    blueTeam->getPlayerClosestToBall()->setPlayerHomeRegion(regions[13].getCenter());
    blueTeam->getReceivingPlayer()->setPlayerHomeRegion(regions[14].getCenter());
    blueTeam->getSupportingPlayer()->setPlayerHomeRegion(regions[20].getCenter());
    blueTeam->getGoalKeeper()->setPlayerHomeRegion(regions[15].getCenter());
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

clsSoccerTeam *clsSoccerPitch::getRedTeam()
{
    return redTeam;
}

clsSoccerTeam *clsSoccerPitch::getBlueTeam()
{
    return blueTeam;
}
void clsSoccerPitch::update()
{
    // Freeze the world once a team wins.
    if (!_gameOn)
    {
        return;
    }

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

    // Resolve player/keeper overlap so entities don't pass through each other.
    std::vector<BodyProxy> bodies;
    if (redTeam != nullptr)
    {
        for (auto *player : redTeam->getFieldPlayers())
        {
            if (player != nullptr)
                bodies.push_back({player, 18.0});
        }
        if (redTeam->getGoalKeeper() != nullptr)
            bodies.push_back({redTeam->getGoalKeeper(), 24.0});
    }
    if (blueTeam != nullptr)
    {
        for (auto *player : blueTeam->getFieldPlayers())
        {
            if (player != nullptr)
                bodies.push_back({player, 18.0});
        }
        if (blueTeam->getGoalKeeper() != nullptr)
            bodies.push_back({blueTeam->getGoalKeeper(), 24.0});
    }
    resolveBodyOverlaps(bodies);

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
    if (ball != nullptr)
    {
        ball->placeBallAtPosition(clsVector2d(_width / 2.0, _height / 2.0));
    }

    if (redTeam != nullptr)
    {
        redTeam->setControllingPlayer(nullptr);
        redTeam->stateMachine.changeState(new PrepareForKickOff());
    }

    if (blueTeam != nullptr)
    {
        blueTeam->setControllingPlayer(nullptr);
        blueTeam->stateMachine.changeState(new PrepareForKickOff());
    }

}

void clsSoccerPitch::render()
{
    int w = static_cast<int>(_width);
    int h = static_cast<int>(_height);

    // Vertical gradient base.
    for (int y = 0; y < h; ++y)
    {
        double t = static_cast<double>(y) / std::max(1, h - 1);
        int r = static_cast<int>(56 + 16 * t);
        int g = static_cast<int>(149 + 34 * t);
        int b = static_cast<int>(96 + 17 * t);
        SDL_SetRenderDrawColor(_renderer, r, g, b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(_renderer, 0, y, w, y);
    }

    // Faint horizontal turf stripes.
    int stripeCount = 10;
    int stripeHeight = h / stripeCount;
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    for (int i = 0; i < stripeCount; ++i)
    {
        if (i % 2 == 0)
        {
            SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 14);
            SDL_Rect stripe = {0, i * stripeHeight, w, stripeHeight};
            SDL_RenderFillRect(_renderer, &stripe);
        }
    }

    // Pitch markings.
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    SDL_Rect pitchBoundary = {2, 2, w - 4, h - 4};
    SDL_RenderDrawRect(_renderer, &pitchBoundary);

    int cx = w / 2;
    int cy = h / 2;
    SDL_RenderDrawLine(_renderer, cx, 0, cx, h);

    // Center circle.
    int centerRadius = h / 7;
    for (int a = 0; a < 360; ++a)
    {
        double rad = a * 0.017453292519943295;
        int x = cx + static_cast<int>(std::cos(rad) * centerRadius);
        int y = cy + static_cast<int>(std::sin(rad) * centerRadius);
        SDL_RenderDrawPoint(_renderer, x, y);
    }
    SDL_Rect centerSpot = {cx - 3, cy - 3, 6, 6};
    SDL_RenderFillRect(_renderer, &centerSpot);

    // Draw goal areas and penalty boxes around each goal line (photo-like layout).
    if (redGoal != nullptr && blueGoal != nullptr)
    {
        int leftGoalX = (int)redGoal->getLeftPost().getX();
        int rightGoalX = (int)blueGoal->getLeftPost().getX();

        int leftGoalTop = std::min((int)redGoal->getLeftPost().getY(), (int)redGoal->getRightPost().getY());
        int leftGoalBottom = std::max((int)redGoal->getLeftPost().getY(), (int)redGoal->getRightPost().getY());
        int rightGoalTop = std::min((int)blueGoal->getLeftPost().getY(), (int)blueGoal->getRightPost().getY());
        int rightGoalBottom = std::max((int)blueGoal->getLeftPost().getY(), (int)blueGoal->getRightPost().getY());

        int penaltyDepth = w / 7;
        int penaltyHeight = h / 2;
        int goalBoxDepth = w / 16;
        int goalBoxHeight = h / 5;

        SDL_Rect leftPenalty = {leftGoalX, cy - penaltyHeight / 2, penaltyDepth, penaltyHeight};
        SDL_Rect rightPenalty = {rightGoalX - penaltyDepth, cy - penaltyHeight / 2, penaltyDepth, penaltyHeight};
        SDL_RenderDrawRect(_renderer, &leftPenalty);
        SDL_RenderDrawRect(_renderer, &rightPenalty);

        SDL_Rect leftGoalBox = {leftGoalX, cy - goalBoxHeight / 2, goalBoxDepth, goalBoxHeight};
        SDL_Rect rightGoalBox = {rightGoalX - goalBoxDepth, cy - goalBoxHeight / 2, goalBoxDepth, goalBoxHeight};
        SDL_RenderDrawRect(_renderer, &leftGoalBox);
        SDL_RenderDrawRect(_renderer, &rightGoalBox);

        // Goal posts/bodies: left is red team side, right is blue team side.
        SDL_SetRenderDrawColor(_renderer, 220, 60, 60, SDL_ALPHA_OPAQUE);
        SDL_Rect leftGoalVisual = {leftGoalX - 6, leftGoalTop, 12, std::max(1, leftGoalBottom - leftGoalTop)};
        SDL_RenderFillRect(_renderer, &leftGoalVisual);

        SDL_SetRenderDrawColor(_renderer, 35, 80, 220, SDL_ALPHA_OPAQUE);
        SDL_Rect rightGoalVisual = {rightGoalX - 6, rightGoalTop, 12, std::max(1, rightGoalBottom - rightGoalTop)};
        SDL_RenderFillRect(_renderer, &rightGoalVisual);

        // Back to white for remaining markings.
        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
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

    // Render soccer ball last so it stays visible above players.
    if (ball != nullptr)
    {
        ball->render(_renderer, 0, 0, 0); // Black ball
    }

    // Match result HUD at top-center.
    drawScoreHud(_renderer, static_cast<int>(_width), redScore, blueScore, !_gameOn);

    SDL_RenderPresent(_renderer);
}
