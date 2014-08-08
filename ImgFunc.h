#include "Vec3.h"
#include "Vec2.h"
#include <opencv2/opencv.hpp>

#ifndef H_IMGFUNC
#define H_IMGFUNC

namespace ImgFunc
{
    Vec3 getPixel(uint8_t* data, int x, int y, int rows, int cols, int channels);
    int getPixelStart(int x, int y, int rows, int cols, int channels);
    Vec2 getCoordsFromPixel(int pixel, int rows);
}
#endif
