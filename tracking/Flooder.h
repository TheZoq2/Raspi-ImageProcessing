#include <vector>
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

    void flood(bool target);
    struct Blob
    {
        Vec2 center;
        float width;
        float height;
        int pixelAmount;
    };
    
    std::vector<Blob> getBlobs();
private:
    Blob searchFrom(int x, int y, bool target);

    Array2d<bool> map;

    std::vector<Blob> blobs;
};
#endif
