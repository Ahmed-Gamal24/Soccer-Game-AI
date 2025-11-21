#pragma once 

#include"vector2d.h"

class clsBaseGameEntity{
private:
    int id;
    clsVector2d position;
    void setId(int id);
    
    public:
    static int nextValidId;
    clsBaseGameEntity(int id);
    int getId();
    virtual void update();//virtual = all entities must implement update function
    void render();//draw visual representation of current state
};