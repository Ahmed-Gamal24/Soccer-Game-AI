
#include"baseGameEntity.h"

int clsBaseGameEntity::nextValidId = 0;

void clsBaseGameEntity::setId(int ID){
    id = ID;
}

int clsBaseGameEntity::getId(){
    return id;
}

clsBaseGameEntity::clsBaseGameEntity(int id, clsVector2d position, double radius, clsVector2d scale){
    setId(id);
    this->position = position;
    this->boundingRadius = radius;
    this->scale = scale;
}

void clsBaseGameEntity::update(){}

void clsBaseGameEntity::render(){}