#include"../include/region.h"

clsRegion::clsRegion(int id, clsVector2d topLeft, clsVector2d bottomRight){
    _id = id;
    _topLeft = topLeft;
    _bottomRight = bottomRight;
}

int clsRegion::nextValidId = 1;
int clsRegion::nextValidID(){
    return nextValidId++;
}

clsRegion::clsRegion(){}

void clsRegion::setID(int id){
    _id = id;
}

int clsRegion::getID(){
    return _id;
}

void clsRegion::setTopLeft(clsVector2d topLeft){
    _topLeft = topLeft;
}

clsVector2d clsRegion::getTopLeft(){
    return _topLeft;
}

void clsRegion::setBottomRight(clsVector2d bottomRight){
    _bottomRight = bottomRight;
}

clsVector2d clsRegion::getBottomRight(){
    return _bottomRight;
}

void clsRegion::setCenter(){
    //calc center point
    _center.setX( (_topLeft.getX() + _bottomRight.getX()) / 2 );
    _center.setY( (_topLeft.getY() + _bottomRight.getY()) / 2 );
    
}

clsVector2d clsRegion::getCenter(){
    return _center;
}

void clsRegion::setRegPos(clsVector2d topLeft, clsVector2d bottomRight){
    setTopLeft(topLeft);
    setBottomRight(bottomRight);
    setCenter();
    setID(nextValidID());

}
