#include "fieldPlayer.h"
#include "fieldPlayerStates.h"

clsFieldPlayer::clsFieldPlayer()
    : clsPlayerBase(clsBaseGameEntity::nextValidId++)
{
    stateMachine.setCurrentState(new GlobalPlayerState());
}


void clsFieldPlayer::update(){
    stateMachine.update();
}

void clsFieldPlayer::render(SDL_Renderer *renderer) {
    if (!renderer) return;
    
    // Triangle size (in pixels)
    const double triangleSize = 15.0;
    
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
    
    // Set color (you can change this - using blue for players)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    
    // Draw triangle outline (3 lines) - SDL_RenderLine accepts float, so cast at call site
    SDL_RenderLine(renderer, (float)tipX, (float)tipY, (float)leftX, (float)leftY);    // Tip to left
    SDL_RenderLine(renderer, (float)tipX, (float)tipY, (float)rightX, (float)rightY);  // Tip to right
    SDL_RenderLine(renderer, (float)leftX, (float)leftY, (float)rightX, (float)rightY); // Base line
}

bool clsFieldPlayer::handleMessage(clsTelegram telegram){}