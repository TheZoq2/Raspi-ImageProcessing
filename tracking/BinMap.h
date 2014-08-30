#include "ImgFunc.h"

#ifndef H_BINMAP
#define H_BINMAP

class BinMap
{
public:
    BinMap(int width, int height);
    
    void setValue(int x, int y, bool value);
private:
    int mWidth;
    int mHeight;

    bool* mData;

    //The amount of elements that are 1 or 0
    int mAmount0;
    int mAmount1;
};
#endif
