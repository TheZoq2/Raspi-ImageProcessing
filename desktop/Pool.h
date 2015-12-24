#ifndef H_POOL
#define H_POOL

#include "objectTracking/Vec2.h"

class Pool
{
public:
    void setWhiteBall(Vec2 whiteBall);
    void setTargetBall(Vec2 targetBall);
    void setPocket(Vec2 hole);

    float getHitAngle();
    float getTargetAngle();
    Vec2 getWhiteBall();
    Vec2 getTargetBall();
private:
    static const float BALL_RADIUS;
    Vec2 whiteBall;
    Vec2 targetBall;
    Vec2 pocket;

    float lastHoleAngle;
};
#endif
