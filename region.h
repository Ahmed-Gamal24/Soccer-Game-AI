#pragma once 

#include"vector2d.h"

class clsRegion{
private:
    int _id;
    clsVector2d _topLeft;
    clsVector2d _bottomRight;
    clsVector2d _center;

public:
    clsRegion(int id, clsVector2d topLeft, clsVector2d bottomRight);

    void setID(int id);
    int getID();

    void setTopLeft(clsVector2d topLeft);
    clsVector2d getTopLeft();

    void setBottomRight(clsVector2d bottomRight);
    clsVector2d getBottomRight();

};