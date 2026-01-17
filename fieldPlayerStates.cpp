#include "fieldPlayerStates.h"
#include "fieldPlayer.h"
#include "telegram.h"
#include "soccerTeam.h"
#include "soccerBall.h"
#include "soccerPitch.h"
#include "playerBase.h"
#include "steeringBehavior.h"

// GlobalPlayerState implementation
void GlobalPlayerState::Enter(clsFieldPlayer *entity)
{
    // Global state doesn't have entry actions typically
}

void GlobalPlayerState::Execute(clsFieldPlayer *entity)
{
    // Global state logic - runs regardless of current state
    // Check for boundary wraparound (if ball goes out of bounds)
    clsSoccerBall *ball = entity->soccerPitch->getBall();
    if (ball != nullptr)
    {
        // Check if ball is way out of bounds and return it
        double pitchWidth = entity->soccerPitch->getWidth();
        double pitchHeight = entity->soccerPitch->getHeight();
        if (ball->position.getX() < -100 || ball->position.getX() > pitchWidth + 100 ||
            ball->position.getY() < -100 || ball->position.getY() > pitchHeight + 100)
        {
            // Ball is out of bounds, reset it
            ball->placeBallAtPosition(clsVector2d(pitchWidth / 2, pitchHeight / 2));
        }
    }
}

void GlobalPlayerState::Exit(clsFieldPlayer *entity)
{
    // Global state doesn't exit typically
}

bool GlobalPlayerState::OnMessage(clsFieldPlayer *entity, const clsTelegram &msg)
{
    // Handle messages that should be processed regardless of current state
    switch (msg.msg)
    {
    case Msg_ReceiveBall:
        // If receiving a pass request, transition to ReceiveBall state
        entity->stateMachine.changeState(new ReceiveBall());
        return true;
    default:
        return false;
    }
}

// ChaseBall state implementation
void ChaseBall::Enter(clsFieldPlayer *entity)
{
    // Player decides to chase the ball
    // Set steering behavior to seek the ball
    // (No special setup needed - Execute will handle seeking)
}

void ChaseBall::Execute(clsFieldPlayer *entity)
{
    // Get the ball from the pitch
    clsSoccerBall *ball = entity->soccerPitch->getBall();
    if (ball == nullptr)
        return;

    // Calculate distance to ball
    double distanceToBall = entity->position.distance(ball->position);

    // Control radius - how close player needs to be to trap the ball
    const double CONTROL_RADIUS = 75.0;

    // Distance at which to start giving up on the chase
    const double GIVE_UP_DISTANCE = 400.0;

    // Check if ball is owned by a teammate
    bool ballOwnedByTeammate = false;
    clsPlayerBase *ballOwner = ball->getBallOwner();
    if (ballOwner != nullptr)
    {
        // Check if ball owner is this player
        if (ballOwner != entity)
        {
            ballOwnedByTeammate = true;
        }
    }

    // Transition to Dribble if player gains control of ball
    if (distanceToBall < CONTROL_RADIUS && !ballOwnedByTeammate)
    {
        ball->trap(entity); // Player now owns the ball
        entity->stateMachine.changeState(new Dribble());
        return;
    }

    // Transition back to ReturnToHomeRegion if ball is too far or controlled by teammate
    if (distanceToBall > GIVE_UP_DISTANCE || ballOwnedByTeammate)
    {
        entity->stateMachine.changeState(new ReturnToHomeRegion());
        return;
    }

    // Seek the ball position
    if (entity->steerBehv != nullptr)
    {
        clsVector2d steeringForce = entity->steerBehv->seek(ball->position);
        // The steering behavior automatically gets integrated into the entity's movement
        // through the update() method
    }
}

void ChaseBall::Exit(clsFieldPlayer *entity)
{
    // Stop chasing behavior
    // (Steering behavior will be overridden by the next state)
}

bool ChaseBall::OnMessage(clsFieldPlayer *entity, const clsTelegram &msg)
{
    switch (msg.msg)
    {
    case Msg_ReceiveBall:
        // If asked to receive a pass, transition to ReceiveBall state
        entity->stateMachine.changeState(new ReceiveBall());
        return true;
    case Msg_GoHome:
        // If told to go home, transition to ReturnToHomeRegion
        entity->stateMachine.changeState(new ReturnToHomeRegion());
        return true;
    default:
        return false;
    }
}

// Dribble state implementation
void Dribble::Enter(clsFieldPlayer *entity)
{
    // Player has control of the ball
    // Trap the ball (already done when transitioning from ChaseBall)
}

void Dribble::Execute(clsFieldPlayer *entity)
{
    clsSoccerBall *ball = entity->soccerPitch->getBall();
    if (ball == nullptr)
        return;

    // If ball is no longer owned by this player, go back to chase
    if (ball->getBallOwner() != entity)
    {
        entity->stateMachine.changeState(new ChaseBall());
        return;
    }

    // Define goal position based on team color
    // Get pitch dimensions
    double pitchWidth = entity->soccerPitch->getWidth();
    double pitchHeight = entity->soccerPitch->getHeight();
    clsVector2d goalPos;

    // We need to determine which team this player belongs to
    // For now, use a heuristic: if x < width/2, attacking right goal; else attacking left goal
    if (entity->position.getX() < pitchWidth / 2.0)
    {
        // Red team - attack toward right (blue goal)
        goalPos = clsVector2d(pitchWidth - 100, pitchHeight / 2.0);
    }
    else
    {
        // Blue team - attack toward left (red goal)
        goalPos = clsVector2d(100, pitchHeight / 2.0);
    }

    // Shoot distance
    const double SHOOT_DISTANCE = 150.0;

    // If in shooting range, don't dribble further (prepare to shoot)
    // For now, just move toward goal
    double distanceToGoal = entity->position.distance(goalPos);

    // Dribble toward the goal
    if (entity->steerBehv != nullptr)
    {
        clsVector2d steeringForce = entity->steerBehv->seek(goalPos);
    }

    // If lose the ball to opponent pressure, go home
    double distanceToBall = entity->position.distance(ball->position);
    if (distanceToBall > 100.0)
    {
        entity->stateMachine.changeState(new ReturnToHomeRegion());
        return;
    }
}

void Dribble::Exit(clsFieldPlayer *entity)
{
    // Release ball control
    clsSoccerBall *ball = entity->soccerPitch->getBall();
    if (ball != nullptr && ball->getBallOwner() == entity)
    {
        ball->setBallOwner(nullptr); // Release ownership
    }
}

bool Dribble::OnMessage(clsFieldPlayer *entity, const clsTelegram &msg)
{
    switch (msg.msg)
    {
    case Msg_GoHome:
        entity->stateMachine.changeState(new ReturnToHomeRegion());
        return true;
    default:
        return false;
    }
}

// ReceiveBall state implementation
void ReceiveBall::Enter(clsFieldPlayer *entity)
{
    // Player is preparing to receive a pass
    // Position stored in message or use current position as target
}

void ReceiveBall::Execute(clsFieldPlayer *entity)
{
    clsSoccerBall *ball = entity->soccerPitch->getBall();
    if (ball == nullptr)
        return;

    // Calculate distance to ball
    double distanceToBall = entity->position.distance(ball->position);

    // Control radius to trap the ball
    const double CONTROL_RADIUS = 75.0;
    const double RECEIVE_TIMEOUT = 5.0; // Seconds to wait for pass

    // If ball comes close enough, trap it
    if (distanceToBall < CONTROL_RADIUS)
    {
        ball->trap(entity); // Player now owns the ball
        entity->stateMachine.changeState(new Dribble());
        return;
    }

    // Move to intercept the ball using pursuit behavior
    // Calculate where ball will be and move toward it
    clsVector2d ballVelocity = ball->velocity;
    double ballSpeed = ballVelocity.length();

    if (ballSpeed > 0.1)
    {
        // Ball is moving - use pursuit (intercept)
        if (entity->steerBehv != nullptr)
        {
            // Estimate where ball will be
            clsVector2d futurePos = ball->position;
            double timeToIntercept = distanceToBall / (entity->maxSpeed + 0.1);
            futurePos.operator+=(ballVelocity.operator*=(timeToIntercept));

            // Seek the future position
            entity->steerBehv->seek(futurePos);
        }
    }
    else
    {
        // Ball is stationary - just seek it
        if (entity->steerBehv != nullptr)
        {
            entity->steerBehv->seek(ball->position);
        }
    }
}

void ReceiveBall::Exit(clsFieldPlayer *entity)
{
    // Done receiving
}

bool ReceiveBall::OnMessage(clsFieldPlayer *entity, const clsTelegram &msg)
{
    switch (msg.msg)
    {
    case Msg_ReceiveBall:
        // Update receive position if message contains new target
        return true;
    case Msg_GoHome:
        // If told to go home, abort receive
        entity->stateMachine.changeState(new ReturnToHomeRegion());
        return true;
    default:
        return false;
    }
}

// SupportAttacker state implementation
void SupportAttacker::Enter(clsFieldPlayer *entity)
{
    // Player moves to support the attacking player
    // Find best supporting position (already determined by team)
}

void SupportAttacker::Execute(clsFieldPlayer *entity)
{
    clsSoccerBall *ball = entity->soccerPitch->getBall();
    if (ball == nullptr)
        return;

    // Get the position of the attacking player (controlling player)
    clsPlayerBase *controllingPlayer = nullptr;
    if (entity->soccerPitch != nullptr)
    {
        // For now, find another player with the ball
        // In a full implementation, get from team's controlling player
        controllingPlayer = ball->getBallOwner();
    }

    // If no one has the ball or it's us, return home
    if (controllingPlayer == nullptr || controllingPlayer == entity)
    {
        entity->stateMachine.changeState(new ReturnToHomeRegion());
        return;
    }

    // Find supporting position - offset from ball, between ball and goal
    double pitchWidth = entity->soccerPitch->getWidth();
    clsVector2d ballPos = ball->position;

    // Calculate support position (behind attacker, ready for pass)
    clsVector2d supportPos = ballPos;
    double offsetDistance = 200.0; // Distance to position ourselves

    // Determine which goal we're attacking
    if (entity->position.getX() < pitchWidth / 2.0)
    {
        // Red team attacking right goal - position to the right of ball
        supportPos.operator+=(clsVector2d(offsetDistance, 0));
    }
    else
    {
        // Blue team attacking left goal - position to the left of ball
        supportPos.operator+=(clsVector2d(-offsetDistance, 0));
    }

    // Move to support position using arrive for smooth approach
    if (entity->steerBehv != nullptr)
    {
        entity->steerBehv->arrive(supportPos, clsSteeringBehavior::Deceleration::normal);
    }

    // If ball is close and we can take it, transition to Dribble
    double distanceToBall = entity->position.distance(ball->position);
    if (distanceToBall < 100.0 && ball->getBallOwner() == nullptr)
    {
        ball->trap(entity);
        entity->stateMachine.changeState(new Dribble());
        return;
    }

    // If no longer supporting (ball lost or far), return home
    if (distanceToBall > 500.0)
    {
        entity->stateMachine.changeState(new ReturnToHomeRegion());
        return;
    }
}

void SupportAttacker::Exit(clsFieldPlayer *entity)
{
    // Stop supporting
}

bool SupportAttacker::OnMessage(clsFieldPlayer *entity, const clsTelegram &msg)
{
    switch (msg.msg)
    {
    case Msg_ReceiveBall:
        // If asked to receive pass, transition to ReceiveBall
        entity->stateMachine.changeState(new ReceiveBall());
        return true;
    case Msg_GoHome:
        // If told to go home, transition to ReturnToHomeRegion
        entity->stateMachine.changeState(new ReturnToHomeRegion());
        return true;
    default:
        return false;
    }
}

// ReturnToHomeRegion state implementation
void ReturnToHomeRegion::Enter(clsFieldPlayer *entity)
{
    // Player returns to their assigned home region
    // (Setup happens in Execute)
}

void ReturnToHomeRegion::Execute(clsFieldPlayer *entity)
{
    // Get home region
    clsVector2d homeRegion = entity->getPlayerHomeRegion();

    // Use arrive behavior to smoothly move to home and decelerate
    if (entity->steerBehv != nullptr)
    {
        clsVector2d steeringForce = entity->steerBehv->arrive(homeRegion, clsSteeringBehavior::Deceleration::normal);
    }

    // Check if reached home region
    const double HOME_REGION_RADIUS = 50.0;
    double distanceToHome = entity->position.distance(homeRegion);

    if (distanceToHome < HOME_REGION_RADIUS)
    {
        // Reached home - transition to Wait
        entity->stateMachine.changeState(new Wait());
        return;
    }

    // If ball becomes close while returning home, switch to chasing
    clsSoccerBall *ball = entity->soccerPitch->getBall();
    if (ball != nullptr)
    {
        double distanceToBall = entity->position.distance(ball->position);
        if (distanceToBall < 200.0)
        {
            entity->stateMachine.changeState(new ChaseBall());
            return;
        }
    }
}

void ReturnToHomeRegion::Exit(clsFieldPlayer *entity)
{
    // Reached home region
}

bool ReturnToHomeRegion::OnMessage(clsFieldPlayer *entity, const clsTelegram &msg)
{
    switch (msg.msg)
    {
    case Msg_SupportAttacker:
        // If team needs support, transition to SupportAttacker
        entity->stateMachine.changeState(new SupportAttacker());
        return true;
    case Msg_ReceiveBall:
        // If asked to receive pass, transition to ReceiveBall
        entity->stateMachine.changeState(new ReceiveBall());
        return true;
    default:
        return false;
    }
}

// Wait state implementation
void Wait::Enter(clsFieldPlayer *entity)
{
    // Player waits at their position
    // Set velocity to zero
    entity->velocity.zero();
}

void Wait::Execute(clsFieldPlayer *entity)
{
    // Stay near home position using arrive behavior
    clsVector2d homeRegion = entity->getPlayerHomeRegion();

    if (entity->steerBehv != nullptr)
    {
        // Use arrive with slow deceleration to stay near position
        entity->steerBehv->arrive(homeRegion, clsSteeringBehavior::Deceleration::slow);
    }

    // Check if ball is close enough to start chasing
    clsSoccerBall *ball = entity->soccerPitch->getBall();
    if (ball != nullptr)
    {
        double distanceToBall = entity->position.distance(ball->position);

        // Transition to ChaseBall if ball is near and not owned
        const double CHASE_DISTANCE = 200.0; // Lower threshold
        if (distanceToBall < CHASE_DISTANCE && ball->getBallOwner() == nullptr)
        {
            entity->stateMachine.changeState(new ChaseBall());
            return;
        }
    }
}

void Wait::Exit(clsFieldPlayer *entity)
{
    // Done waiting
}

bool Wait::OnMessage(clsFieldPlayer *entity, const clsTelegram &msg)
{
    switch (msg.msg)
    {
    case Msg_SupportAttacker:
        entity->stateMachine.changeState(new SupportAttacker());
        return true;
    case Msg_ReceiveBall:
        entity->stateMachine.changeState(new ReceiveBall());
        return true;
    default:
        return false;
    }
}
