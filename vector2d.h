#pragma once


const double EPSILON = 0.00000001; 
class clsVector2d{
private:
    double _x;
    double _y;

public:
    clsVector2d();
    clsVector2d(double x, double y);

    void setX(double x);
    double getX()const;
    void setY(double y);
    double getY()const;
    void zero();

    bool isZero();
    double length();
    double squaredLength();
    void normalize();


    /*return the result of dot product
       - if positive -> the angle between vectors is less than 90 
       -- (generaly thay point to the same direction)
       -- the target vector in front of the viewing vector

       - if zero -> two vectors are perpendicular
       -- the target is exactly to the side of the viewing direction

       - if positive -> the angle between vectors is greater than 90 (generaly thay point to opposite directions)
       -- the target vector is behind the viewing vector
    */
    double dot(const clsVector2d& v2); 


    /*returns positive if v2 is clockwise of this vector,
      negative if counterclockwise */
    int sign(const clsVector2d& v2);

    //returns the vector that is perpendicular to this vector(current object)
    inline clsVector2d perp()const;

    //adjusts x and y so that the length of the vector does not exceed max
    void truncate(double max);

    //squared version of above
    inline double distanceSq(const clsVector2d &v2)const;
    
    //returns the distance between this vector and the one passed as a parameter
    inline double distance(const clsVector2d &v2)const;


    //returns the vector that is the reverse of this vector
    inline clsVector2d GetReverse()const;

    //we need some operators
    const clsVector2d& operator+=(const clsVector2d &rhs);
    const clsVector2d& operator-=(const clsVector2d &rhs);
    const clsVector2d& operator*=(const double& rhs);
    clsVector2d& operator=(const clsVector2d& rhs);
    const clsVector2d& operator/=(const double& rhs);
    bool operator==(const clsVector2d& rhs)const;
    bool operator!=(const clsVector2d& rhs)const;
};