#include "../include/fieldPlayer.h"
#include "../include/fieldPlayerStates.h"

clsFieldPlayer::clsFieldPlayer()
    : clsPlayerBase(clsBaseGameEntity::nextValidId++),
      stateMachine(this)
{
    stateMachine.setGlobalState(new GlobalPlayerState());
    stateMachine.setCurrentState(new Wait());
}

void clsFieldPlayer::update()
{
    // First update the state machine to execute player behaviors (which set steering targets)
    stateMachine.updateStateMachine();

    // Then apply physics (steering forces, movement)
    clsMovingEntity::update(1.0 / 60.0); // 60 FPS
}

void clsFieldPlayer::render(SDL_Renderer *renderer)
{
    if (!renderer)
        return;

    // Triangle size (in pixels)
    const double triangleSize = 50.0;

    // Get player position
    double posX = position.getX();
    double posY = position.getY();

    // Calculate triangle points based on heading and side vectors
    // If heading is not initialized, default to pointing right
    double headingX = !vHeading.isZero() ? vHeading.getX() : 1.0;
    double headingY = !vHeading.isZero() ? vHeading.getY() : 0.0;
    double sideX = !vSide.isZero() ? vSide.getX() : 0.0;
    double sideY = !vSide.isZero() ? vSide.getY() : 1.0;

    // Front tip of triangle (pointing in heading direction)
    double tipX = posX + headingX * triangleSize;
    double tipY = posY + headingY * triangleSize;

    // Back points of triangle (base)
    double backX = posX - headingX * (triangleSize * 0.5);
    double backY = posY - headingY * (triangleSize * 0.5);

    double leftX = backX + sideX * (triangleSize * 0.5);
    double leftY = backY + sideY * (triangleSize * 0.5);

    double rightX = backX - sideX * (triangleSize * 0.5);
    double rightY = backY - sideY * (triangleSize * 0.5);

    // Set color using the player's color properties
    SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, SDL_ALPHA_OPAQUE);

    // Draw triangle outline (3 lines) - SDL_RenderDrawLine accepts int, so cast at call site
    SDL_RenderDrawLine(renderer, (int)tipX, (int)tipY, (int)leftX, (int)leftY);     // Tip to left
    SDL_RenderDrawLine(renderer, (int)tipX, (int)tipY, (int)rightX, (int)rightY);   // Tip to right
    SDL_RenderDrawLine(renderer, (int)leftX, (int)leftY, (int)rightX, (int)rightY); // Base line
}

bool clsFieldPlayer::handleMessage(clsTelegram telegram)
{
    return stateMachine.handleMessage(telegram);
}