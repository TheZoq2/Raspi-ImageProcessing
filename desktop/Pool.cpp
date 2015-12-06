#include "Pool.h"

void Pool::setWhiteBall(Vec2 whiteBall)
{
    this->whiteBall = whiteBall;
}
void Pool::setTargetBall(Vec2 targetBall)
{
    this->targetBall = targetBall;
}
void Pool::setPocket(Vec2 hole)
{
    this->hole = hole;
}

float Pool::getHitAngle() 
{
    //Calculating the angle of the vector that the target needs to travel
    Vec2 targetDiff = hole - targetBall;
}
