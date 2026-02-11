#pragma once 

#include "vector2d.h"

class clsRegion{
private:
    int _id;
    clsVector2d _topLeft;
    clsVector2d _bottomRight;
    clsVector2d _center;
    
public:

    static int nextValidId;
    static int nextValidID();

    clsRegion(int id, clsVector2d topLeft, clsVector2d bottomRight);
    clsRegion();

    void setID(int id);
    int getID();

    void setTopLeft(clsVector2d topLeft);
    clsVector2d getTopLeft();

    void setBottomRight(clsVector2d bottomRight);
    clsVector2d getBottomRight();

    void setCenter();
    clsVector2d getCenter();

    void setRegPos(clsVector2d topLeft, clsVector2d bottomRight);

};