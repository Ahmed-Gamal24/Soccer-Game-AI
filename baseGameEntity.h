#pragma once 

#include"vector2d.h"

class clsBaseGameEntity{
    public:
        int id;
        double boundingRadius;
        bool tag;
       
        clsVector2d scale;
        clsVector2d position;

        void setId(int id);
    
    public:
        static int nextValidId;

        clsBaseGameEntity(int id);
        int getId();
        void update();//virtual = all entities must implement update function
        void render();//draw visual representation of current state
};