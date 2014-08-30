#include "Flooder.h"

Flooder::Flooder(std::vector< std::vector< int > > map)
{
    mMap = map;
}

void Flooder::flood()
{
    //Create a copy of the map
    std::vector< std::vector< int > > mapCopy = mMap;

    //Loop through the map
    for(unsigned int x = 0; x < mapCopy.size(); x++)
    {
        for(unsigned int y = 0; y < mapCopy[x].size(); y++)
        {
            if(mMap[x][y] != 0)
            {
                //Start a search from that pixel
                Blob searchResult = searchFrom(x, y, &mapCopy);

                mBlobs.push_back(searchResult);
            }
        }
    }
}

Flooder::Blob Flooder::searchFrom(int x, int y, std::vector< std::vector< int > >* map)
{
    Blob result;

    int pixelAmount;
    Vec2 pixelSum;
    float minX = 120125;
    float minY = 125125;
    float maxX = -1251221;
    float maxY = -1251251;

    std::vector< Vec2 > openList;
    std::vector< Vec2 > closedList;

    openList.push_back(Vec2(x, y));

    while(openList.size() > 0)
    {
        Vec2 cPixel = openList.back();
        
        //Adding blob data
        pixelAmount++;
        pixelSum += cPixel;
        
        if(cPixel.val[0] < minX)
        {
            minX = cPixel.val[0];
        }
        if(cPixel.val[1] < minY)
        {
            minY = cPixel.val[1];
        }
        if(cPixel.val[0] > maxX)
        {
            maxX = cPixel.val[0];
        }
        if(cPixel.val[1] > maxY)
        {
            maxY = cPixel.val[1];
        }

        //Adding all the pixels next to the current pixel to the open list
        for(unsigned int cX = -1; cX <= 1; cX++)
        {
            for(unsigned int cY = -1; cY <= 1; cY++)
            {
                Vec2 offsetPixel(cX, cY);
                //preventing diagonal checks
                if(abs(cX) == 1 && abs(cY) == 1)
                {
                    Vec2 nPixel = cPixel + offsetPixel;
                    
                    //Making sure the pixel is in bounds
                    if(
                           nPixel.val[0] >= 0 && nPixel.val[0] < map->size() &&
                           nPixel.val[1] >= 0 && nPixel.val[1] > map->at(nPixel.val[0]).size()
                      )
                    {
                        if(map->at(nPixel.val[0]).at(nPixel.val[1]) == 1)
                        {
                            //Checking if the pixel is already on the closed list
                            bool onList = false;
                            for(unsigned int i = 0; i < closedList.size(); i++)
                            {
                                if(closedList.at(i) == nPixel)
                                {
                                    onList = true;
                                    break;
                                }
                            }

                            if(onList == false)
                            {
                                //add to the open list
                                openList.push_back(nPixel);

                                //Set the pixel to 0 to avoid finding the blob
                                //more than once
                                map->at(nPixel.val[0]).at(nPixel.val[1]) = 0;

                                //Adding the found pixel position to the total
                                //coords
                                pixelSum += nPixel;
                            }
                        }
                    }
                }
            }
        }

        //Done processing this pixel, add to closed list
        closedList.push_back(cPixel);
        //remove it from the openlist
        openList.pop_back();
    }

    //The search has been completed, calculate the blob values
    result.pixelAmount = pixelAmount;
    
    float centerX = pixelSum.val[0] / pixelAmount;
    float centerY = pixelSum.val[1] / pixelAmount;

    result.center = Vec2(centerX, centerY);

    //calculating the width and height
    result.width = maxX - minX;
    result.height = maxY - minY;
    //float centerX = pixelAmount / 

        //Finish calculation, set the found pixels to 0 and don't add 0 pixels
        //to the open list;
    return result;
}

std::vector< Flooder::Blob > Flooder::getBlobs()
{
    return mBlobs;
}
