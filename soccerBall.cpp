#include "soccerBall.h"
#include "math.h"

clsSoccerBall::clsSoccerBall(clsVector2d position, double radius,
                  double ballSize, double mass,
                 clsWall* pitchWalls):
                  clsMovingEntity(clsBaseGameEntity::nextValidId++,
                     position, radius, clsVector2d(1.0, 1.0), mass, -1.0, 0, 0)
{
    this->pitchWalls = pitchWalls;
    this->ballOwner = nullptr;
}

void clsSoccerBall::trap(clsPlayerBase *owner)
{
    velocity.zero();
    ballOwner = owner;
}

clsVector2d clsSoccerBall::getOldPos()
{
    return oldPosition;
}

clsVector2d clsSoccerBall::futurePosition(double time){
    //using the equation x = ut + 1/2at^2, where x = distance, a = friction
    // Calculate displacement due to current velocity: velocity * time
    clsVector2d velocityDisplacement = this->velocity;
    velocityDisplacement.operator*=(time);
    
    // Calculate displacement due to friction: 0.5 * friction * time^2 in velocity direction
    double halfATSquared = 0.5 * fricion * time * time;
    clsVector2d frictionDirection = this->velocity;
    
    // Only apply friction if velocity is not zero
    if (!frictionDirection.isZero()) {
        frictionDirection.normalize();
        frictionDirection.operator*=(halfATSquared);
    }
    
    // Calculate future position: current position + velocity displacement + friction displacement
    clsVector2d futurePos = this->position;
    futurePos.operator+=(velocityDisplacement);
    futurePos.operator+=(frictionDirection);
    
    return futurePos;
}


void clsSoccerBall::render(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b) 
{
    // Validate inputs
    if (!renderer || boundingRadius <= 0) {
        return;
    }
    
    // Use ball's own position and radius
    float center_x = (float)position.getX();
    float center_y = (float)position.getY();
    float radius = (float)boundingRadius;
    
    // Set the drawing color for the circle
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);

    // Midpoint Circle Algorithm for filled circle using float coordinates
    float x = radius;
    float y = 0.0f;
    float radiusError = 1.0f - x;

    // The loop calculates one octant and uses symmetry to draw the rest with horizontal lines.
    while (x >= y) 
    {
        // Draw horizontal lines across the circle's width at the current y step.
        // This process fills the interior of the circle using SDL3's float-based rendering.
        
        // Top half
        SDL_RenderLine(renderer, center_x - x, center_y - y, center_x + x, center_y - y);
        SDL_RenderLine(renderer, center_x - y, center_y - x, center_x + y, center_y - x);

        // Bottom half
        SDL_RenderLine(renderer, center_x - x, center_y + y, center_x + x, center_y + y);
        SDL_RenderLine(renderer, center_x - y, center_y + x, center_x + y, center_y + x);

        // Update the error term to find the next point
        y++;
        if (radiusError < 0) 
        {
            radiusError += 2.0f * y + 1.0f;
        } else {
            x--;
            radiusError += 2.0f * (y - x) + 1.0f;
        }
    }
}

double clsSoccerBall::timeToCoverDistance(clsVector2d A, clsVector2d B, double force)const
{
    //this will be the velocity of the ball in the next time step *if*
    //the player was to make the pass
    double speed = force / mass;
    //calculate the velocity at B using the equation
    //
    // v^2 = u^2 + 2ax
    //
    //first calculate s (the distance between the two positions)
    double distanceToCover = A.distance(B);
    double term = speed*speed + 2.0*distanceToCover*fricion;
    //if (u^2 + 2ax) is negative it means the ball cannot reach point B.
    if (term <= 0) return -1.0;
    double v = sqrt(term);
    //it’s possible for the ball to reach B and we know its speed when it
    //gets there, so now it's easy to calculate the time using the equation
    //
    // t = v-u
    // ---
    // a
    //
    return (v-speed)/fricion;
}