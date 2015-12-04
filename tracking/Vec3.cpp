#include <stdio.h>

#include "Vec3.h"

Vec3::Vec3()
{
    val[0] = 0;
    val[1] = 0;
    val[2] = 0;
}
Vec3::Vec3(float x, float y, float z)
{
    val[0] = x;
    val[1] = y;
    val[2] = z;
}

std::string Vec3::getString()
{

    char buffer[256];
    sprintf(buffer, "[%f, %f, %f]", val[0], val[1], val[2]);

    std::string result = buffer;

    return result;
}

float Vec3::x()
{
    return val[0];
}
float Vec3::y()
{
    return val[1];
}
float Vec3::z()
{
    return val[2];
}

Vec3& Vec3::operator=(Vec3 arg)
{
    this->val[0] = arg.val[0];
    this->val[1] = arg.val[1];
    this->val[2] = arg.val[2];
    return *this;
}
Vec3 Vec3::operator+=(const Vec3 rhs)
{
    this->val[0] = this->val[0] + rhs.val[0];
    this->val[1] = this->val[1] + rhs.val[1];
    this->val[2] = this->val[2] + rhs.val[2];

    return *this;
}
Vec3 Vec3::operator-=(const Vec3 rhs)
{
    this->val[0] = this->val[0] - rhs.val[0];
    this->val[1] = this->val[1] - rhs.val[1];
    this->val[2] = this->val[2] - rhs.val[2];

    return *this;
}
const Vec3 Vec3::operator+(const Vec3& rhs)
{
    Vec3 result = *this;
    result += rhs;
    return result;
}
const Vec3 Vec3::operator-(const Vec3& rhs)
{
    Vec3 result = *this;
    result -= rhs;
    return result;
}
