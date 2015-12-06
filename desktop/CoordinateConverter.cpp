#include "CoordinateConverter.h"

#include <math.h>

CoordinateConverter::CoordinateConverter(std::vector<Vec2> corners)
{
    this->corners = corners;

    Vec2 xVector = corners[1] - corners[0];

    normalLength = xVector.getLength();

    angle = -xVector.getAngle();

    startCorner = corners[0];
}

Vec2 CoordinateConverter::convertTo(Vec2 coords)
{
    Vec2 result;

    coords -= startCorner;

    result.val[0] = coords.val[0] * cos(angle) - coords.val[1] * sin(angle);
    result.val[1] = coords.val[0] * sin(angle) + coords.val[1] * cos(angle);


    result *= 1/normalLength;

    return result;
}

Vec2 CoordinateConverter::convertFrom(Vec2 coords)
{
    Vec2 result;


    result.val[0] = coords.val[0] * cos(-angle) - coords.val[1] * sin(-angle);
    result.val[1] = coords.val[0] * sin(-angle) + coords.val[1] * cos(-angle);

    result *= normalLength;

    result += startCorner;

    return result;
}
