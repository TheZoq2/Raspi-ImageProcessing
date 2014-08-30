#include "BinMap.h"

BinMap::BinMap(int width, int height)
{
    mWidth = width;
    mHeight = height;

    mData = new bool[width * height];

    int mAmount0 = width * height;
    int mAmount1 = 0;
}

void BinMap::setValue(int x, int y, bool value)
{
    if(x < 0 || x > mWidth)
    {
        std::cout << "Failed to set binmap value, x: " << x << " < " << mWidth << std::endl;
        return;
    }
    if(y < 0 || y > mHeight)
    {
        std::cout << "Failed to set binmap value, y: " << y << " < " << mHeight << std::endl;
        return;
    }

    int pixel = ImgFunc::getPixelStart(x, y, mWidth, mHeight, 1);

    bool oldVal = mData[pixel];
    mData[pixel] = value;

    //Changing the value amount
    if(oldVal != value)
    {
        if(value == true)
        {
            mAmount1++;
            mAmount0--;
        }
        if(value == false)
        {
            mAmount1--;
            mAmount0++;
        }
    }
}

