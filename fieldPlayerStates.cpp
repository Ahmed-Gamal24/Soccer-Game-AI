#include "fieldPlayerStates.h"
#include "fieldPlayer.h"
#include "telegram.h"
#include "soccerTeam.h"
#include "soccerBall.h"
#include "soccerPitch.h"
#include "playerBase.h"
#include "steeringBehavior.h"

// GlobalPlayerState implementation
void GlobalPlayerState::Enter(clsFieldPlayer* entity) {
    // Global state doesn't have entry actions typically
}

void GlobalPlayerState::Execute(clsFieldPlayer* entity) {
    // Global state logic - runs regardless of current state
    // Could check for important game events, update common player info, etc.
}

void GlobalPlayerState::Exit(clsFieldPlayer* entity) {
    // Global state doesn't exit typically
}

bool GlobalPlayerState::OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) {
    // Handle messages that should be processed regardless of current state
    switch(msg.msg) {
        case Msg_ReceiveBall:
            // If receiving a pass request, can handle here or in current state
            return true;
        default:
            return false;
    }
}

// ChaseBall state implementation
void ChaseBall::Enter(clsFieldPlayer* entity) {
    // Player decides to chase the ball
    // Set steering behavior to seek the ball
}

void ChaseBall::Execute(clsFieldPlayer* entity) {
    // Continuously move towards the ball
    // Check if ball is close enough to control
    // Transition to Dribble if player gains control
    // Transition to ReturnToHomeRegion if ball is too far or controlled by teammate
}

void ChaseBall::Exit(clsFieldPlayer* entity) {
    // Stop chasing behavior
}

bool ChaseBall::OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) {
    switch(msg.msg) {
        case Msg_ReceiveBall:
            // If asked to receive a pass, transition to ReceiveBall state
            return true;
        case Msg_GoHome:
            // If told to go home, transition to ReturnToHomeRegion
            return true;
        default:
            return false;
    }
}

// Dribble state implementation
void Dribble::Enter(clsFieldPlayer* entity) {
    // Player has control of the ball
    // Decide where to dribble (towards goal, to create space, etc.)
}

void Dribble::Execute(clsFieldPlayer* entity) {
    // Move with the ball
    // Check if should pass or shoot
    // If opponent is too close, may want to pass
    // If in good shooting position, may want to shoot
}

void Dribble::Exit(clsFieldPlayer* entity) {
    // Release ball control
}

bool Dribble::OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) {
    // Handle messages while dribbling
    return false;
}

// ReceiveBall state implementation
void ReceiveBall::Enter(clsFieldPlayer* entity) {
    // Player is preparing to receive a pass
    // Move to intercept position
}

void ReceiveBall::Execute(clsFieldPlayer* entity) {
    // Move to position to receive the ball
    // Check if ball is arriving
    // When ball arrives, trap it and transition to Dribble
}

void ReceiveBall::Exit(clsFieldPlayer* entity) {
    // Done receiving
}

bool ReceiveBall::OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) {
    switch(msg.msg) {
        case Msg_ReceiveBall:
            // Update receive position if message contains new target
            return true;
        default:
            return false;
    }
}

// SupportAttacker state implementation
void SupportAttacker::Enter(clsFieldPlayer* entity) {
    // Player moves to support the attacking player
    // Find best supporting position
}

void SupportAttacker::Execute(clsFieldPlayer* entity) {
    // Move to optimal supporting position
    // Stay in good position for a pass
    // Adjust position based on ball and teammates
}

void SupportAttacker::Exit(clsFieldPlayer* entity) {
    // Stop supporting
}

bool SupportAttacker::OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) {
    switch(msg.msg) {
        case Msg_ReceiveBall:
            // If asked to receive pass, transition to ReceiveBall
            return true;
        case Msg_GoHome:
            // If told to go home, transition to ReturnToHomeRegion
            return true;
        default:
            return false;
    }
}

// ReturnToHomeRegion state implementation
void ReturnToHomeRegion::Enter(clsFieldPlayer* entity) {
    // Player returns to their assigned home region
}

void ReturnToHomeRegion::Execute(clsFieldPlayer* entity) {
    // Move towards home region
    // Once at home region, may transition to Wait or SupportAttacker
}

void ReturnToHomeRegion::Exit(clsFieldPlayer* entity) {
    // Reached home region
}

bool ReturnToHomeRegion::OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) {
    switch(msg.msg) {
        case Msg_SupportAttacker:
            // If team needs support, transition to SupportAttacker
            return true;
        case Msg_ReceiveBall:
            // If asked to receive pass, transition to ReceiveBall
            return true;
        default:
            return false;
    }
}

// Wait state implementation
void Wait::Enter(clsFieldPlayer* entity) {
    // Player waits at their position
}

void Wait::Execute(clsFieldPlayer* entity) {
    // Stay at position
    // Observe the game
    // May transition to ChaseBall if ball is close
    // May transition to SupportAttacker if team has ball
}

void Wait::Exit(clsFieldPlayer* entity) {
    // Done waiting
}

bool Wait::OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) {
    switch(msg.msg) {
        case Msg_SupportAttacker:
            return true;
        case Msg_ReceiveBall:
            return true;
        default:
            return false;
    }
}

