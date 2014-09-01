#include "ImgFunc.h"

Vec3 ImgFunc::getPixel(uint8_t* data, int x, int y, int rows, int cols, int channels)
{
    Vec3 result(0, 0, 0);

    int rowIndex = (y-1) * cols * channels;
    int pixelStart = rowIndex + x * channels;

    for(int i = 0; i < channels; i++)
    {
        result.val[i] = data[pixelStart + i];
    }

    return result;
}
int ImgFunc::getPixelStart(int x, int y, int rows, int cols, int channels)
{
    int rowIndex = (y-1) * rows * channels;
    int pixelStart = rowIndex + x * channels;

    return pixelStart;
}

Vec2 ImgFunc::getCoordsFromPixel(int pixel, int rows, int cols)
{
    Vec2 result;
    result.val[0] = (pixel % cols) + 1;
    result.val[1] = floor(pixel /(float) cols);

    return result;
}
