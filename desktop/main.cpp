#include <stdio.h>

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <unistd.h>
#include <memory>

#include "objectTracking/ColorTracker.h"
#include "objectTracking/Flooder.h"
#include "CoordinateConverter.h"

enum PROGRAM_STATE
{
    SELECT_COLOR,
    SELECT_BALL,
};

cv::VideoCapture camera(1);

Vec3 targetColor(29, 227, 218);
Vec3 minColor;
Vec3 maxColor;
Vec3 redColor(1, 227, 218);
Vec3 minRed;
Vec3 maxRed;

Vec3 threshold(30, 80, 80);

std::deque< Flooder::Blob > blobs;

bool running = true;

//Used as a safe way of shutting down the program since ctrl+c doesn't stop
//using the webcam
void onMouse(int event, int x, int y, int, void*);
void selectColor(int event, int x, int y, int , void*);
void setMinMaxColor(Vec3 target, Vec3& minColor, Vec3& maxColor, Vec3 threshold);


std::unique_ptr<CoordinateConverter> coordinateConverter;

int main()
{
    //Reading bounds of the table
    std::ifstream boundFile;
    boundFile.open("bounds");

    std::vector<Vec2> boundCoords;
    for(int i = 0; i < 3; ++i)
    {
        float x, y;

        boundFile >> x >> y;

        boundCoords.push_back(Vec2(x, y));
    }
    coordinateConverter.reset(new CoordinateConverter(boundCoords));
    
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

    cv::namedWindow("GUI");
    cv::setMouseCallback("GUI", onMouse);
    cv::moveWindow("GUI", 0, 500);

    cv::namedWindow("Display Image");
    cv::setMouseCallback("Display Image", selectColor);

    cv::namedWindow("Threshold img");
    cv::moveWindow("Threshold img", 0, 0);

    while(running == true)
    {
        cv::Mat img;
        camera >> img;

        //Fix fisheye problems
        cv::Mat tmpImg;

        //Read fisheye correction stuff from a file. Generate using the "cpp/calibration" sample
        cv::Mat camera_matrix, distortion;
        cv::FileStorage fs("fisheye0.txt", cv::FileStorage::READ);
        cv::FileNode fn = fs["IntParam"];
        fn["camera_matrix"] >> camera_matrix;
        fn["distortion"] >> distortion;

        cv::undistort(img, tmpImg, camera_matrix, distortion);

        img = tmpImg;
        
        //clock_t startTime = clock();

        //Colortracking
        ColorTracker ct;
        ct.setImage(img);
        ct.generateBinary(minColor, maxColor, true);

        //Getting the blob data from the trackers
        ct.generateBlobs();
        blobs = ct.getBlobs(60);

        int maxBlobSize = 0;
        auto maxBlob= blobs.begin();
        for(auto it = blobs.begin(); it != blobs.end(); ++it)
        {
            if(it->pixelAmount > maxBlobSize)
            {
                maxBlobSize = it->pixelAmount;
                maxBlob= it;
            }
        }
        blobs.erase(maxBlob);

        for(unsigned int i = 0; i < blobs.size(); i++)
        {
            ct.drawCircle(blobs.at(i).center, 5, cv::Scalar(255, 0, 0));
        }
        
        cv::imshow("Threshold img", ct.getBinary());
        cv::imshow("Display Image", img);

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
    if(event == CV_EVENT_RBUTTONDOWN) //If this isn't a left click
    {
        //Taking 1 image
        cv::Mat ref;
        camera >> ref;

        //Create a color tracker to get the color
        ColorTracker ct;
        ct.setImage(ref);

        redColor = ct.getColorInPixel(Vec2(x, y));
        
        //Recalculating the threshold
        setMinMaxColor(redColor, minColor, maxColor, threshold);
    }
    else
    {
        auto lowestBlob = blobs.begin();
        float lowestDistance = std::numeric_limits<float>::max();
        for(auto it = blobs.begin(); it != blobs.end(); ++it)
        {
            Vec2 distance = (it->center - Vec2(x,y));
            if(distance.getLength() < lowestDistance)
            {
                lowestDistance = distance.getLength();
                lowestBlob = it;
            }
        }

        std::cout << coordinateConverter->convertTo(Vec2(x,y)).getString() << std::endl;
    }
}


void setMinMaxColor(Vec3 target, Vec3& minColor, Vec3& maxColor, Vec3 threshold)
{
    minColor = target - threshold;
    maxColor = target + threshold;
}
