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
#include "Pool.h"

enum PROGRAM_STATE
{
    WAIT_FOR_SELECT,
    SELECT_PARAMETERS,
    PLAY
};
PROGRAM_STATE state;

enum SELECT_STATE
{
    WHITE_BALL,
    TARGET,
    HOLE
};
SELECT_STATE selState;

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
void captureNewImage();
void runSelect(float x, float y);

void mainLoop();

std::unique_ptr<CoordinateConverter> coordinateConverter;

cv::Mat currentImage;
ColorTracker ct;

Pool pool;

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
        mainLoop();
    }

    return 0;
}

void mainLoop()
{
    if(state == WAIT_FOR_SELECT)
    {
        captureNewImage();

        //Colortracking
        ct.setImage(currentImage);
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
    }
    else if(state == SELECT_PARAMETERS)
    {

    }
    else if(state == PLAY)
    {
        //state = WAIT_FOR_SELECT;
        captureNewImage();

        //Colortracking
        ct.setImage(currentImage);
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

        //calculatng the line that we want to draw
        const float lineLength = 100;
        float hitAngle = pool.getHitAngle();

        Vec2 lineStart(lineLength * cos(hitAngle), lineLength * sin(hitAngle));
        Vec2 lineEnd(lineLength * cos(hitAngle + M_PI), lineLength * sin(hitAngle + M_PI));

        //Adding those coordinates to the location of the white ball
        Vec2 whitePos = coordinateConverter->convertFrom(pool.getWhiteBall());
        lineStart += whitePos;
        lineEnd += whitePos;

        std::cout << lineStart.getString() << std::endl;

        cv::line(
                currentImage, 
                cv::Point(lineStart.val[0], lineStart.val[1]), 
                cv::Point(lineEnd.val[0], lineEnd.val[1]),
                cv::Scalar(0,0,255),
                3,
                CV_AA
                );
    }

    for(unsigned int i = 0; i < blobs.size(); i++)
    {
        ct.drawCircle(blobs.at(i).center, 5, cv::Scalar(255, 0, 0));
    }
    
    cv::imshow("Threshold img", ct.getBinary());
    cv::imshow("Display Image", currentImage);

    cv::waitKey(1);
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
        //Create a color tracker to get the color
        ColorTracker ct;
        ct.setImage(currentImage);

        redColor = ct.getColorInPixel(Vec2(x, y));
        
        //Recalculating the threshold
        setMinMaxColor(redColor, minColor, maxColor, threshold);
    }
    else if(event == CV_EVENT_LBUTTONDOWN)
    {
        switch(state) 
        {
            case WAIT_FOR_SELECT:
            {
                state = SELECT_PARAMETERS;
                break;
            }
            case SELECT_PARAMETERS:
            {
                runSelect(x, y);
                break;
            }
        }
    }
    else
    {
    }
}


void setMinMaxColor(Vec3 target, Vec3& minColor, Vec3& maxColor, Vec3 threshold)
{
    minColor = target - threshold;
    maxColor = target + threshold;
}

void captureNewImage()
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
    currentImage = tmpImg;
}

void runSelect(float x, float y)
{
    //Finding the closest ball
    float minBlobDistance = 0;
    auto closestBlob = blobs.begin();
    for(auto it = blobs.begin(); it != blobs.end(); ++it)
    {
        Vec2 diff = closestBlob->center - Vec2(x, y);

        if(diff.getLength() < minBlobDistance)
        {
            closestBlob = it;
        }
    }

    Vec2 closestCenter = coordinateConverter->convertTo(closestBlob->center);

    switch(selState) 
    {
        case WHITE_BALL:
        {
            pool.setWhiteBall(closestCenter);
            selState = TARGET;
            break;
        }
        case TARGET:
        {
            pool.setTargetBall(closestCenter);

            selState = HOLE;
            break;
        }
        case HOLE:
        {
            float holeX = round(x * 2) / 2.0;
            float holeY = round(y * 2) / 2.0;

            if(holeX < 0)
                holeX = 0;
            if(holeX > 1)
                holeX = 1;
            if(holeY < 0)
                holeY = 0;
            if(holeY > 0.5)
                holeX = 0.5;

            pool.setPocket(coordinateConverter->convertTo(Vec2(holeX, holeY)));
            selState = WHITE_BALL;
            state = PLAY;
            break;
        }
    }
}
