#include"vector2d.h"
#include"math.h"

clsVector2d::clsVector2d(){
    _x = 0;
    _y = 0;
}


clsVector2d::clsVector2d(double x, double y){
    _x = x;
    _y = y;
}

void clsVector2d::setX(double x){
    _x = x;
}
double clsVector2d::getX(){
    return _x;
}

void clsVector2d::setY(double y){
    _y = y;
}
double clsVector2d::getY(){
    return _y;
}

void clsVector2d::zero(){
    _x = 0;
    _y = 0;
}




// void clsVector2d::normalize(){
//     float magnitude = sqrt( pow(_x, 2) + pow(_y, 2) );
//         _x = _x / magnitude;
//         _y = _y / magnitude;
// }