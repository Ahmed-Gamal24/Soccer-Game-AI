#include "../include/soccerBall.h"
#include "../include/playerBase.h"
#include "../include/math.h"
#include <cmath>

namespace
{
void drawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius)
{
    for (int dy = -radius; dy <= radius; ++dy)
    {
        int span = (int)std::sqrt((double)(radius * radius - dy * dy));
        SDL_RenderDrawLine(renderer, cx - span, cy + dy, cx + span, cy + dy);
    }
}
}

clsSoccerBall::clsSoccerBall(clsVector2d position, double radius,
                             double ballSize, double mass,
                             clsWall *pitchWalls) : clsMovingEntity(clsBaseGameEntity::nextValidId++,
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

void clsSoccerBall::kick(clsVector2d direction, int force)
{
    // Normalize direction and apply force
    if (!direction.isZero())
    {
        direction.normalize();
        velocity = direction;
        velocity.operator*=(force / mass);
    }
    ballOwner = nullptr; // Ball is no longer owned after being kicked
}

clsVector2d clsSoccerBall::getOldPos()
{
    return oldPosition;
}

void clsSoccerBall::placeBallAtPosition(clsVector2d newPos)
{
    position = newPos;
    velocity.zero();
    ballOwner = nullptr;
}

void clsSoccerBall::testCollisionWithWalls(clsWall *pitchWalls)
{
    // TODO: Implement proper wall collision detection
    // For now, just a placeholder - should check each wall and reflect velocity
    if (pitchWalls == nullptr)
        return;

    // Basic implementation: check if ball is outside pitch bounds
    // This is simplified - proper implementation would check each wall segment
}

void clsSoccerBall::update(double time_elapsed)
{
    // Store old position before updating
    oldPosition = position;

    // If ball is owned by a player, move with player
    if (ballOwner != nullptr)
    {
        // ballOwner is clsPlayerBase*, which inherits from clsMovingEntity, which inherits from clsBaseGameEntity
        // position is in clsBaseGameEntity (public member), so we can access it
        // Use a temporary to avoid potential access issues
        clsVector2d ownerPos;
        ownerPos = ballOwner->position; // This should work since position is public in baseGameEntity
        position = ownerPos;
        velocity.zero();
        return;
    }

    // Update ball physics (friction, velocity)
    if (!velocity.isZero())
    {
        // Apply friction to velocity
        clsVector2d frictionForce = velocity;
        if (!frictionForce.isZero())
        {
            frictionForce.normalize();
            frictionForce.operator*=(fricion * time_elapsed);
            velocity.operator+=(frictionForce);
        }

        // Stop ball if velocity is very small
        if (velocity.length() < 0.1)
        {
            velocity.zero();
        }
    }

    // Update position based on velocity
    if (!velocity.isZero())
    {
        clsVector2d velCopy = velocity;
        velCopy.operator*=(time_elapsed);
        position.operator+=(velCopy);
    }

    // Test collision with walls (if implemented)
    // Note: testCollisionWithWalls needs to be implemented separately
}

clsVector2d clsSoccerBall::futurePosition(double time)
{
    // using the equation x = ut + 1/2at^2, where x = distance, a = friction
    //  Calculate displacement due to current velocity: velocity * time
    clsVector2d velocityDisplacement = this->velocity;
    velocityDisplacement.operator*=(time);

    // Calculate displacement due to friction: 0.5 * friction * time^2 in velocity direction
    double halfATSquared = 0.5 * fricion * time * time;
    clsVector2d frictionDirection = this->velocity;

    // Only apply friction if velocity is not zero
    if (!frictionDirection.isZero())
    {
        frictionDirection.normalize();
        frictionDirection.operator*=(halfATSquared);
    }

    // Calculate future position: current position + velocity displacement + friction displacement
    clsVector2d futurePos = this->position;
    futurePos.operator+=(velocityDisplacement);
    futurePos.operator+=(frictionDirection);

    return futurePos;
}

void clsSoccerBall::render(SDL_Renderer *renderer, uint8_t r, uint8_t g, uint8_t b)
{
    (void)r;
    (void)g;
    (void)b;

    if (!renderer || boundingRadius <= 0)
        return;

    int cx = (int)position.getX();
    int cy = (int)position.getY();
    int radius = (int)(boundingRadius * 1.6);
    if (radius < 9)
        radius = 9;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 75);
    drawFilledCircle(renderer, cx + 2, cy + 3, radius);

    SDL_SetRenderDrawColor(renderer, 245, 245, 245, SDL_ALPHA_OPAQUE);
    drawFilledCircle(renderer, cx, cy, radius);

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
    for (int a = 0; a < 360; ++a)
    {
        double rad = a * 0.017453292519943295;
        int x = cx + (int)(std::cos(rad) * radius);
        int y = cy + (int)(std::sin(rad) * radius);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    // Simple black panel pattern for soccer-ball look.
    drawFilledCircle(renderer, cx, cy, radius / 3);
    drawFilledCircle(renderer, cx + radius / 2, cy, radius / 4);
    drawFilledCircle(renderer, cx - radius / 2, cy, radius / 4);
    drawFilledCircle(renderer, cx, cy + radius / 2, radius / 4);
    drawFilledCircle(renderer, cx, cy - radius / 2, radius / 4);
}

double clsSoccerBall::timeToCoverDistance(clsVector2d A, clsVector2d B, double force) const
{
    // this will be the velocity of the ball in the next time step *if*
    // the player was to make the pass
    double speed = force / mass;
    // calculate the velocity at B using the equation
    //
    //  v^2 = u^2 + 2ax
    //
    // first calculate s (the distance between the two positions)
    double distanceToCover = A.distance(B);
    double term = speed * speed + 2.0 * distanceToCover * fricion;
    // if (u^2 + 2ax) is negative it means the ball cannot reach point B.
    if (term <= 0)
        return -1.0;
    double v = sqrt(term);

    // it's possible for the ball to reach B and we know its speed when it
    // gets there, so now it's easy to calculate the time using the equation
    //
    //  t = v-u
    //  ---
    //  a
    //
    return (v - speed) / fricion;
}

clsPlayerBase *clsSoccerBall::getBallOwner() const
{
    return ballOwner;
}

void clsSoccerBall::setBallOwner(clsPlayerBase *owner)
{
    ballOwner = owner;
}
