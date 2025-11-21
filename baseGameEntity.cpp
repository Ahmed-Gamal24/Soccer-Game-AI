
#include"baseGameEntity.h"

int clsBaseGameEntity::nextValidId = 0;

void clsBaseGameEntity::setId(int ID){
    id = ID;
}

int clsBaseGameEntity::getId(){
    return id;
}

clsBaseGameEntity::clsBaseGameEntity(int id){
    setId(id);
}