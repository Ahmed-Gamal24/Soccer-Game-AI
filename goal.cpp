
#include <SDL3/SDL.h>
#include"vector2d.h"
#include"goal.h"
#include"math.h"

clsGoal::clsGoal(clsVector2d left, clsVector2d right):clsBaseGameEntity(clsBaseGameEntity::nextValidId++){
    leftPost = left;
    rightPost = right;

}

clsVector2d clsGoal::getLeftPost(){
    return leftPost;
}
clsVector2d clsGoal::getRightPost(){
    return rightPost;
}

void clsGoal::render(SDL_Renderer* renderer){
    SDL_RenderLine(renderer,leftPost.getX(), leftPost.getY(), rightPost.getX(), rightPost.getY());
     
}

