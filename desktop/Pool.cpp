#include "Pool.h"

#include <iostream>
#include <math.h>

const float Pool::BALL_RADIUS = 0.5/(15 * 2);

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

    lastHoleAngle = targetAngle;

    //Calculating where the white needs to be when it hits
    Vec2 whiteTarget(BALL_RADIUS * 2 * cos(targetAngle + M_PI), BALL_RADIUS * 2 * sin(targetAngle + M_PI));
    whiteTarget += targetBall;

    Vec2 finalDiff = whiteTarget - whiteBall; 

    return finalDiff.getAngle();

}
Vec2 Pool::getWhiteBall()
{
    return whiteBall;
}

float Pool::getTargetAngle()
{
    return lastHoleAngle;
}
Vec2 Pool::getTargetBall()
{
    return targetBall;
}
