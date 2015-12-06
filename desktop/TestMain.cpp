#include <iostream>
#include "Pool.h"
#include "objectTracking/Vec2.h"

int main() {
    Vec2 testWhite = Vec2(1,0.5);
    Vec2 testTarget = Vec2(0.5,0.25);
    Vec2 testPocket = Vec2(0,0);
    Pool pool;
    pool.setWhiteBall(testWhite);
    pool.setTargetBall(testTarget);
    pool.setPocket(testPocket);

    std::cout << pool.getHitAngle() << std::endl;
}
