#include <string>
#include <stdio.h>

#ifndef H_VEC2
#define H_VEC2

class Vec2
{
public:
    Vec2();
    Vec2(float x, float y);

    std::string getString();
    bool getHasValue();
    
    float val[2];

    Vec2& operator=(Vec2 arg);
    Vec2 operator+=(const Vec2& rhs);
    Vec2 operator-=(const Vec2& rhs);
    Vec2 operator*=(const float rhs);
    const Vec2 operator+(const Vec2& rhs);
    const Vec2 operator-(const Vec2& rhs);
    bool operator==(const Vec2& other);

    float getAngle();
    float getLength();
};
#endif
