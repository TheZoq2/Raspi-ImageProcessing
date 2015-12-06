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
    Vec2 getWhiteBall();
private:
    Vec2 whiteBall;
    Vec2 targetBall;
    Vec2 pocket;
};
#endif
