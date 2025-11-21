#include"region.h"

clsRegion::clsRegion(int id, clsVector2d topLeft, clsVector2d bottomRight){
    _id = id;
    _topLeft = topLeft;
    _bottomRight = bottomRight;
    //calc center point
    _center.setX( (_topLeft.getX() + _bottomRight.getX()) / 2 );
    _center.setY( (_topLeft.getY() + _bottomRight.getY()) / 2 );

}