#include "Vec2.h"

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

Vec2& Vec2::operator=(Vec2 arg)
{
    this->val[0] = arg.val[0];
    this->val[1] = arg.val[1];
    return *this;
}
