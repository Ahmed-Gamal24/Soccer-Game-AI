#include"vector2d.h"
#include"math.h"
#include<stdexcept>

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
double clsVector2d::getX()const{
    return _x;
}

void clsVector2d::setY(double y){
    _y = y;
}
double clsVector2d::getY()const{
    return _y;
}

void clsVector2d::zero(){
    _x = 0;
    _y = 0;
}

double clsVector2d::squaredLength(){
    return (_x * _x ) + (_y * _y);
}


bool clsVector2d::isZero()  {
    // Check if the squared length is less than a tiny threshold.
    // This handles precision issues and avoids the sqrt() call.
    return squaredLength() < EPSILON; 
}

double clsVector2d::length(){
    return sqrt(squaredLength());
}

void clsVector2d::normalize(){
    if(!isZero()){
        double magnitude = length();
        _x = _x / magnitude;
        _y = _y / magnitude;
    }
}

clsVector2d clsVector2d::perp()const{
    clsVector2d perpV(-1 * _y , _x);
    return perpV;
}

double clsVector2d::dot(const clsVector2d& v2){
    double result = (_x * v2.getX()) + (_y * v2.getY());
    return result;
}

int clsVector2d::sign(const clsVector2d& v2){
    return ( _x * v2.getX() ) - ( _y * v2.getY() );
}

void clsVector2d::truncate(double max){
    double sqLen = squaredLength();
    if (sqLen > max * max){ //so length > max
        
        //set the direction (by normalizing the vector to length 1)
        this->normalize(); 
        
        //scale it up exactly to the 'max' limit
        this->_x *= max;
        this->_y *= max;
    }
}


const clsVector2d& clsVector2d::operator+=(const clsVector2d &rhs){
    _x += rhs.getX();
    _y += rhs.getY();
    return *this;//return pointer to the currenr object
}

const clsVector2d& clsVector2d::operator-=(const clsVector2d &rhs){
    _x -= rhs.getX();
    _y -= rhs.getY();
    return *this;
}

const clsVector2d& clsVector2d::operator*=(const double& rhs){
    _x *= rhs;
    _y *= rhs;
    return *this;
}

const clsVector2d& clsVector2d::operator/=(const double& rhs){
    if (rhs == 0) throw std::runtime_error("Division by zero in vector.");
    _x /= rhs;
    _y /= rhs;
    return *this;
}

clsVector2d& clsVector2d::operator=(const clsVector2d& rhs){
    this->setX(rhs.getX());
    this->setY(rhs.getY());

    return *this;
}

bool clsVector2d::operator==(const clsVector2d& rhs)const{
    return (std::abs(_x - rhs.getX()) < EPSILON) && 
           (std::abs(_y - rhs.getY()) < EPSILON);
}

bool clsVector2d::operator!=(const clsVector2d& rhs)const{
    return !this->operator==(rhs);
}

inline double clsVector2d::distanceSq(const clsVector2d &v2)const{
    double SqDistance = (this->_x - v2.getX()) + (this->_y - v2.getY());
    return SqDistance;
}

inline double clsVector2d::distance(const clsVector2d &v2)const{
    return sqrt( distanceSq(v2) );
}
