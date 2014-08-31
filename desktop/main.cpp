#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <unistd.h>

#include "objectTracking/ColorTracker.h"
#include "objectTracking/Flooder.h"

cv::VideoCapture camera(0);

Vec3 targetColor(29, 227, 218);
Vec3 minColor;
Vec3 maxColor;
Vec3 redColor(1, 227, 218);
Vec3 minRed;
Vec3 maxRed;

Vec3 threshold(15, 100, 100);

bool running = true;

//Used as a safe way of shutting down the program since ctrl+c doesn't stop
//using the webcam
void onMouse(int event, int x, int y, int, void*);
void selectColor(int event, int x, int y, int , void*);
void setMinMaxColor(Vec3 target, Vec3* minColor, Vec3* maxColor, Vec3 threshold);

int main()
{

    if(camera.isOpened() == false)
    {
        std::cout << "Failed to open camera" << std::endl;
    }
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

    //capture 1 image for use as reference
    cv::Mat ref;
    camera >> ref;

    ColorTracker ct;
    ct.setImage(ref);

    //targetColor = ct.getObjectColor();
    minColor = targetColor - threshold;
    maxColor = targetColor + threshold;
    minRed = redColor - threshold;
    maxRed = redColor + threshold;

    cv::namedWindow("GUI");
    cv::setMouseCallback("GUI", onMouse);

    cv::namedWindow("Display Image");
    cv::setMouseCallback("Display Image", selectColor);

    cv::namedWindow("Threshold img");
    cv::moveWindow("Threshold img", 450, 50);
    cv::namedWindow("red img");
    cv::moveWindow("red img", 50, 450);
    usleep(1000);

    while(running == true)
    {
        cv::Mat img;
        camera >> img;
        
        clock_t startTime = clock();

        //Colortracking
        ColorTracker ct;
        ct.setImage(img);
        ct.generateBinary(minColor, maxColor, true);

        ColorTracker rt;
        rt.setImage(img);
        rt.generateBinary(minRed, maxRed, true);


        //Getting the blob data from the trackers
        rt.generateBlobs();
        std::deque< Flooder::Blob > blobs = rt.getBlobs(0);

        int maxBlobSize = 0;
        for(unsigned int i = 0; i < blobs.size(); i++)
        {
            if(blobs.at(i).pixelAmount > maxBlobSize)
            {
                maxBlobSize = blobs.at(i).pixelAmount;
            }
        }
        std::cout << "The biggset blob is: " << maxBlobSize << std::endl;
        
        clock_t endTime = clock();

        std::cout << "Search took: " << (float)(endTime-startTime)/CLOCKS_PER_SEC << std::endl;
        std::cout << "Found: " << blobs.size() << " blobs" << std::endl;

        cv::imshow("Threshold img", ct.getBinary());
        cv::imshow("Display Image", img);
        cv::imshow("red img", rt.getBinary());

        cv::waitKey(10);
    }

    return 0;
}

void onMouse(int event, int x, int y, int, void*)
{
    if(event != 1)
    {
        return;
    }
    
    //Exiting the program
    running = false;
}

void selectColor(int event, int x, int y, int, void*)
{
    if(event != 1) //If this isn't a left click
    {
        return;
    }

    //Taking 1 image
    cv::Mat ref;
    camera >> ref;

    //Create a color tracker to get the color
    ColorTracker ct;
    ct.setImage(ref);

    redColor = ct.getColorInPixel(Vec2(x, y));
    
    //Recalculating the threshold
    setMinMaxColor(redColor, &minRed, &maxRed, threshold);
}


void setMinMaxColor(Vec3 target, Vec3* minColor, Vec3* maxColor, Vec3 threshold)
{
    *minColor = target - threshold;
    *maxColor = target + threshold;
}