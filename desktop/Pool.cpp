#include "Pool.h"

#include <iostream>
#include <math.h>

const float Pool::BALL_RADIUS = 0.5/40;

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
    std::cout << whiteBall.getString() << std::endl;
    //Calculating the angle of the vector that the target needs to travel
    Vec2 targetDiff = pocket - targetBall;

    float targetAngle = targetDiff.getAngle();

    //Calculating where the white needs to be when it hits
    Vec2 whiteTarget(BALL_RADIUS * cos(targetAngle - M_PI), BALL_RADIUS * sin(targetAngle - M_PI));
    whiteTarget += targetBall;

    Vec2 finalDiff = whiteTarget - whiteBall; 

    return finalDiff.getAngle();

}
Vec2 Pool::getWhiteBall()
{
    return whiteBall;
}
