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
    clsVector2d temp = this->velocity;
    clsVector2d ut = this->velocity.operator*=(time);
    double halfATSquared = 0.5 * fricion * time * time; 
    temp.normalize();
    clsVector2d scalareToVector = temp.operator*=(halfATSquared);
    this->position.operator+=(ut);
    this->position.operator+=(scalareToVector); 

}


void clsSoccerBall::render(SDL_Renderer *renderer)
{
    // simple filled circle using horizontal spans (midpoint circle algorithm)
    int x = this->boundingRadius;
    int y = 0;
    int radiusError = 1 - x;

    int cx = static_cast<int>(std::round(position.getX()));
    int cy = static_cast<int>(std::round(position.getY()));

    while (x >= y)
    {
        // top half
        SDL_RenderLine(renderer, cx - x, cy - y, cx + x, cy - y);
        SDL_RenderLine(renderer, cx - y, cy - x, cx + y, cy - x);

        // bottom half
        SDL_RenderLine(renderer, cx - x, cy + y, cx + x, cy + y);
        SDL_RenderLine(renderer, cx - y, cy + x, cx + y, cy + x);

        y++;
        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        }
        else
        {
            x--;
            radiusError += 2 * (y - x) + 1;
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