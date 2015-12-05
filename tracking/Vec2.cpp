#include "Vec2.h"

#include <math.h>

Vec2::Vec2()
{
    val[0] = -1;
    val[1] = -1;
}

Vec2::Vec2(float x, float y)
{
    val[0] = x;
    val[1] = y;
}

std::string Vec2::getString()
{
    char buffer[256];
    
    sprintf(buffer, "[%f, %f]", val[0], val[1]);

    std::string result = buffer;
    return result;
}
bool Vec2::getHasValue()
{
    if(val[0] == -1 && val[1] == -1)
    {
        return false;
    }
    return true;
}

float Vec2::getAngle() 
{
    return atan2(val[1], val[0]);
}
float Vec2::getLength()
{
    return sqrt(pow(val[0], 2) + pow(val[1], 2));
}

Vec2& Vec2::operator=(Vec2 arg)
{
    this->val[0] = arg.val[0];
    this->val[1] = arg.val[1];
    return *this;
}
Vec2 Vec2::operator+=(const Vec2& rhs)
{
    this->val[0] = this->val[0] + rhs.val[0];
    this->val[1] = this->val[1] + rhs.val[1];

    return *this;
}
Vec2 Vec2::operator-=(const Vec2& rhs)
{
    this->val[0] = this->val[0] - rhs.val[0];
    this->val[1] = this->val[1] - rhs.val[1];

    return *this;
}
Vec2 Vec2::operator*=(const float rhs)
{
    this->val[0] = this->val[0] * rhs;
    this->val[1] = this->val[1] * rhs;

    return *this;
}

const Vec2 Vec2::operator+(const Vec2& rhs)
{
    Vec2 result = *this;
    result += rhs;
    return result;
}
const Vec2 Vec2::operator-(const Vec2& rhs)
{
    Vec2 result = *this;
    result -= rhs;
    return result;
}
bool Vec2::operator==(const Vec2& other)
{
    if(other.val[0] == this->val[0] && other.val[1] == this->val[1])
    {
        return true;
    }
    return false;
}
