#include "Flooder.h"

#include <chrono>

Flooder::Flooder(Array2d<bool> map)
{
    this->map = map;
}

void Flooder::flood(bool target)
{
    //Loop through the map
    for(unsigned int x = 0; x < map.getWidth(); x += 1)
    {
        for(unsigned int y = 0; y < map.getHeight(); y += 1)
        {
            if(map.at(x, y) == target)
            {
                //Start a search from that pixel
                Blob searchResult = searchFrom(x, y, target);

                blobs.push_back(searchResult);
            }
        }
    }
}

Flooder::Blob Flooder::searchFrom(int x, int y, bool target)
{
    Blob result;

    int pixelAmount = 0;
    Vec2 pixelSum(0, 0);
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();

    std::queue< Vec2 > openList;

    openList.push(Vec2(x, y));

    while(openList.size() > 0)
    {
        Vec2 cPixel = openList.front();
        
        //Adding blob data
        pixelAmount++;
        pixelSum += cPixel;
        
        //Adding all the pixels next to the current pixel to the open list
        for(int cX = -1; cX <= 1; cX++)
        {
            for(int cY = -1; cY <= 1; cY++)
            {
                Vec2 offsetPixel(cX, cY);

                
                if(!(abs(cX) == 1 && abs(cY) == 1))
                {
                    Vec2 nPixel = cPixel + offsetPixel;
                    
                    //Making sure the pixel is in bounds
                    if(
                           nPixel.val[0] >= 0 && nPixel.val[0] < map.getWidth() &&
                           nPixel.val[1] >= 0 && nPixel.val[1] < map.getHeight()
                      )
                    {
                        if(map.at(nPixel.val[0], nPixel.val[1]) == target)
                        {
                            //add to the open list
                            openList.push(nPixel);

                            //Set the pixel to 0 to avoid finding the blob
                            //more than once
                            map.at(nPixel.val[0], nPixel.val[1]) = !target;
                        }
                    }
                }
            }
        }

        //remove it from the openlist
        openList.pop();
    }

    //The search has been completed, calculate the blob values
    result.pixelAmount = pixelAmount;
    
    float centerX = pixelSum.val[0] / pixelAmount;
    float centerY = pixelSum.val[1] / pixelAmount;

    result.center = Vec2(centerX, centerY);

    //calculating the width and height
    result.width = maxX - minX;
    result.height = maxY - minY;

    return result;
}

std::vector< Flooder::Blob > Flooder::getBlobs()
{
    return blobs;
}
