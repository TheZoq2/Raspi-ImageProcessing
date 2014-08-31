#include <vector>
#include <deque>
#include <stdlib.h>
#include <iostream>

#include "Vec2.h"

#ifndef H_FLOODER
#define H_FLOODER

class Flooder
{
public:
    Flooder(std::vector< std::vector< int > > map);

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
    Blob searchFrom(int x, int y, std::vector< std::vector< int > >* map);

    std::vector< std::vector< int > > mMap;

    std::deque<Blob> mBlobs;
};
#endif
