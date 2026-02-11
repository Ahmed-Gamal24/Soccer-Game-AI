
#include "../include/vector2d.h"
#include "../include/goal.h"
#include "../include/soccerBall.h"
#include "math.h"
#include <cmath>

clsGoal::clsGoal(clsVector2d left, clsVector2d right) : clsBaseGameEntity(clsBaseGameEntity::nextValidId++,
                                                                          clsVector2d(0, 0), 10, clsVector2d(1, 1))
{
    leftPost = left;
    rightPost = right;
    // Calculate center of goal line
    centerOfGoalLine.setX((left.getX() + right.getX()) / 2.0);
    centerOfGoalLine.setY((left.getY() + right.getY()) / 2.0);
    numGoalScored = 0;
}

clsVector2d clsGoal::getLeftPost()
{
    return leftPost;
}
clsVector2d clsGoal::getRightPost()
{
    return rightPost;
}

bool clsGoal::isScored(clsSoccerBall *ball)
{
    if (ball == nullptr)
        return false;

    // Check if ball position is between the goal posts
    clsVector2d ballPos = ball->position;
    double ballX = ballPos.getX();
    double ballY = ballPos.getY();

    double leftX = leftPost.getX();
    double leftY = leftPost.getY();
    double rightY = rightPost.getY();

    // Check if ball is at the goal line (x coordinate matches)
    // and y coordinate is between the two posts
    if (std::abs(ballX - leftX) < 10.0)
    { // Ball is at goal line (with tolerance)
        double minY = std::min(leftY, rightY);
        double maxY = std::max(leftY, rightY);
        if (ballY >= minY && ballY <= maxY)
        {
            return true;
        }
    }

    return false;
}

void clsGoal::render(SDL_Renderer *renderer)
{
    SDL_RenderDrawLine(renderer, (int)leftPost.getX(), (int)leftPost.getY(), (int)rightPost.getX(), (int)rightPost.getY());
}
