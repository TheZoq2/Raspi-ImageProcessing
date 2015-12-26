#include "ColorTracker.h"
#include "../desktop/Performance.h"

ColorTracker::ColorTracker()
{
    
}

/*void ColorTracker::piGrabImage(raspicam::RaspiCam_Cv* camera)
{
    //Grabbing an image from the camera
    camera->grab();
    camera->retrieve(lastImg);

    //Storing data from the image now to save time in the future
    imgRows = lastImg.rows;
    imgCols = lastImg.cols;

    isConverted = false;
}*/
void ColorTracker::setImage(cv::Mat img)
{
   lastImg.create(img.rows, img.cols, CV_8UC3);
   lastImg.data = img.data;

   imgRows = lastImg.rows;
   imgCols = lastImg.cols;

   isConverted = false;
}

Vec3 ColorTracker::getObjectColor()
{
    return getColorInPixel(Vec2(imgRows / 2, imgCols / 2));
    /*
    Vec3 result;

    //Convert to HSV
    convertToHSV();
    uint8_t* hsvData = hsvImg.data;
    
    Vec3 colorSum;
    
    //Calculating the average color of the object
    int avgSize = 5;
    for(int x = -avgSize; x <= avgSize; x++)
    {
        for(int y = -avgSize; y <= avgSize; y++)
        {
            int pixelX = x + imgRows / 2;
            int pixelY = y + imgRows / 2;
            //std::cout << "processing pixel: " << x << ", " << y << std::endl;
            Vec3 pixel = ImgFunc::getPixel(hsvData, pixelX, pixelY, imgRows, imgCols, 3);

            //Going through all the pixels
            for(int i = 0; i < 3; i++)
            {
                colorSum.val[i] += pixel.val[i];
            }
        }
    }

    int avgSearchAmount = pow(avgSize * 2 + 1, 2);

    for(int i = 0; i < 3; i++)
    {
        result.val[i] = colorSum.val[i] / avgSearchAmount;
    }

    return result;
    */
}
Vec3 ColorTracker::getColorInPixel(Vec2 pixel)
{
    Vec3 result;

    //Convert the image to HSV
    convertToHSV();
    uint8_t* hsvData = hsvImg.data;

    Vec3 colorSum(0, 0, 0);
    int avgSize = 5;
    for(int x = -avgSize; x <= avgSize; x++)
    {
        for(int y = -avgSize; y <= avgSize; y++)
        {
            int pixelX = x + pixel.val[0];
            int pixelY = y + pixel.val[1];

            Vec3 pixel = ImgFunc::getPixel(hsvData, pixelX, pixelY, imgRows, imgCols, 3);

            //Storing the pixel
            colorSum += pixel;
        }
    }

    int avgSearchAmount = pow(avgSize * 2 + 1, 2);
    for(int i = 0; i < 3; i++)
    {
        result.val[i] = colorSum.val[i] / avgSearchAmount;
    }

    return result;
}
void ColorTracker::generateBinary(Vec3 minThresh, Vec3 maxThresh, bool calcMiddle)
{
    
    Performance p;

    p.startMeassurement();
    //Clearing the vector
    
    //binaryMap = Array2d<bool>(lastImg.cols, lastImg.rows);
    binaryMap.resize(lastImg.cols, lastImg.rows);
    binaryMap.initialiseAll(1);

    p.endMeassuremet();
    p.printResult();

    //binaryImg.create(cv::Size(imgRows, imgCols), CV_8UC1);
    binaryImg.create(cv::Size(lastImg.cols, lastImg.rows), CV_8UC1);

    //Converting the image to HSV
    //cv::cvtColor(lastImg, hsvImg, CV_BGR2HSV);
    convertToHSV();

    //Getting the data
    uint8_t* hsvData = hsvImg.data;
    uint8_t* binaryData = binaryImg.data;

    
    Vec2 sumPos; //Stores the sum of the position of found pixels. Used if 
    int pixelsFound = 0;
    
    //Looking for the pixels within the bounds
    for(std::size_t i = 0; i < imgRows * imgCols; ++i)
    {
        bool inBounds = true;
        for(std::size_t n = 0; n < 3; ++n)
        {
            //std::cout << "Searching pixel pos: " << i*3+n << std::endl;
            if(hsvData[i * 3 + n] > maxThresh.val[n] || hsvData[i * 3 + n] < minThresh.val[n])
            {
                inBounds = false;
            }
        }

        //Storing the data
        if(inBounds == true)
        {
            binaryData[i] = 255;

            if(calcMiddle == true) //If the avg middlepoint should be calculated
            {
                pixelsFound++;

                Vec2 pixelCoord = ImgFunc::getCoordsFromPixel(i, imgRows, imgCols);
                sumPos.val[0] += pixelCoord.val[0];
                sumPos.val[1] += pixelCoord.val[1];

                if(pixelCoord.val[0] > 0 && pixelCoord.val[0] < binaryMap.getWidth() && pixelCoord.val[1] > 0 && pixelCoord.val[1] < binaryMap.getHeight())
                {
                    //Adding the pixels to the binary map
                    binaryMap.at(pixelCoord.val[0], pixelCoord.val[1]) = false;
                }
            }
        }
        else
        {
            binaryData[i] = 100;
        }
    }

    //Calculating the average pixel position
    if(calcMiddle == true)
    {
        middlePos.val[0] = sumPos.val[0] / pixelsFound;
        middlePos.val[1] = sumPos.val[1] / pixelsFound;
    }
}
void ColorTracker::drawCircle(Vec2 pos, float radius, cv::Scalar color)
{
    cv::circle(lastImg, cv::Point(pos.val[0], pos.val[1]), radius, color, -1, 8);
}
void ColorTracker::convertToHSV()
{
    //Don't convert if it's alread HSV
    if(isConverted == false)
    {
        cv::cvtColor(lastImg, hsvImg, CV_BGR2HSV);
    }

    isConverted = true;
}
void ColorTracker::generateBlobs()
{
    //std::cout << "Setting image" << std::endl;
    //Creating a new flooder
    Flooder flooder(binaryMap);
    //std::cout << "Set flooder img" << std::endl;
    flooder.flood();
    //std::cout << "Finished flood" << std::endl;

    blobs = flooder.getBlobs();
}
std::deque< Flooder::Blob > ColorTracker::getBlobs(int minSize)
{
    if(minSize == 0)
    {
        return blobs;
    }
    
    //Creating a new vector with the result
    std::deque< Flooder::Blob > result;
    for(unsigned int i = 0; i < blobs.size(); i++)
    {
        //Checking if the blob is big enough
        if(blobs.at(i).pixelAmount > minSize)
        {
            result.push_back(blobs.at(i));
        }
    }

    return result;
}

void ColorTracker::saveImage(std::string filename)
{
    cv::imwrite(filename, lastImg); 
}
void ColorTracker::saveBinary(std::string filename)
{
    cv::imwrite(filename, binaryImg);
}

Vec2 ColorTracker::getMiddlePos()
{
    return middlePos;
}

cv::Mat ColorTracker::getBinary()
{
    return binaryImg;
}
