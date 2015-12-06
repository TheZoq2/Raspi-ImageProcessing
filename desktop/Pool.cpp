#include "Pool.h"

void Pool::setWhiteBall(Vec2 whiteBall)
{
    this->whiteBall = whiteBall;
}
void Pool::setTargetBall(Vec2 targetBall)
{
    this->targetBall = targetBall;
}
void Pool::setPocket(Vec2 pocket)
{
    this->pocket = pocket;
}

float Pool::getHitAngle() 
{
    //Calculating the angle of the vector that the target needs to travel
    Vec2 targetDiff = pocket - targetBall;

    float targetAngle = targetDiff.getAngle();

    return targetAngle;
}
Vec2 Pool::getWhiteBall()
{
    return whiteBall;
}
