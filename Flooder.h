#include <vector>
#include <stdlib.h>

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
    
    std::vector<Blob> getBlobs();
private:
    Blob searchFrom(int x, int y, std::vector< std::vector< int > >* map);

    std::vector< std::vector< int > > mMap;

    std::vector<Blob> mBlobs;
};
#endif
