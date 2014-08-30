#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <unistd.h>

#include "objectTracking/ColorTracker.h"
#include "objectTracking/Flooder.h"

Vec3 targetColor(29, 227, 218);
Vec3 minColor;
Vec3 maxColor;
Vec3 redColor(170, 227, 218);
Vec3 minRed;
Vec3 maxRed;

Vec3 threshold(10, 100, 100);

int main()
{
    cv::VideoCapture camera(0);

    if(camera.isOpened() == false)
    {
        std::cout << "Failed to open camera" << std::endl;
    }

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

    cv::namedWindow("Threshold img");
    cv::moveWindow("Threshold img", 450, 50);
    cv::namedWindow("red img");
    cv::moveWindow("red img", 50, 450);
    usleep(1000);
    bool running = true;
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
        ct.generateBlobs();
        std::vector< Flooder::Blob > blobs = ct.getBlobs();
        
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
