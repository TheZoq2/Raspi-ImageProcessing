#include <vector>
#include <deque>
#include <queue>
#include <stdlib.h>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "Vec2.h"
#include "ImgFunc.h"
#include "Array2d.h"

#ifndef H_FLOODER
#define H_FLOODER

class Flooder
{
public:
    Flooder(Array2d<bool> map);

    void flood();
    struct Blob
    {
        Vec2 center;
        float width;
        float height;
        int pixelAmount;
    };
    
    std::deque<Blob> getBlobs();
private:
    Blob searchFrom(int x, int y);

    Array2d<bool> mMap;

    std::deque<Blob> mBlobs;
};
#endif
