#ifndef H_COORDINATE_CONVERTER
#define H_COORDINATE_CONVERTER

#include "objectTracking/Vec2.h"

#include <vector>

class CoordinateConverter
{
public:
    CoordinateConverter (std::vector<Vec2> corners);

    Vec2 convertTo(Vec2 coords);
    Vec2 convertFrom(Vec2 coords);

    Vec2 getCorner(int id);
private:
    std::vector<Vec2> corners;

    Vec2 startCorner;
    float angle;
    float normalLength;

};
#endif
