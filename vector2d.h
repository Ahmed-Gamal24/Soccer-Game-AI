#pragma once

class clsVector2d{
private:
    double _x;
    double _y;

public:
    clsVector2d();
    clsVector2d(double x, double y);
    void setX(double x);
    double getX();
    void setY(double y);
    double getY();
    void zero();

    // bool isZero();
    // double length();
    // double squaredLength();
    // void normalize();
    // double dot(const clsVector2d& v2); //return the result of dot product
    // //returns positive if v2 is clockwise of this vector,
    //  //negative if counterclockwise (assuming the Y axis is pointing down,
    //   //X axis to right like a Window app) inline int
    // int sign(const clsVector2d& v2);

    // //returns the vector that is perpendicular to this one
    // inline clsVector2d Perp()const;
    // //adjusts x and y so that the length of the vector does not exceed max inline void
    // void Truncate(double max);
    // //returns the distance between this vector and the one passed as a parameter
    // inline double Distance(const clsVector2d &v2)const;
    // //squared version of above
    // inline double DistanceSq(const clsVector2d &v2)const;
    // //returns the vector that is the reverse of this vector
    // inline clsVector2d GetReverse()const;
    // //we need some operators
    // const clsVector2d& operator+=(const clsVector2d &rhs);
    // const clsVector2d& operator-=(const clsVector2d &rhs);
    // const clsVector2d& operator*=(const double& rhs);
    // const clsVector2d& operator/=(const double& rhs);
    // bool operator==(const clsVector2d& rhs)const;
    // bool operator!=(const clsVector2d& rhs)const;
};