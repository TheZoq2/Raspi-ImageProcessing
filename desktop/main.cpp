#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <unistd.h>

#include "objectTracking/ColorTracker.h"
#include "objectTracking/Flooder.h"

cv::VideoCapture camera(1);

Vec3 targetColor(29, 227, 218);
Vec3 minColor;
Vec3 maxColor;
Vec3 redColor(1, 227, 218);
Vec3 minRed;
Vec3 maxRed;

Vec3 threshold(30, 100, 100);

bool running = true;

//Used as a safe way of shutting down the program since ctrl+c doesn't stop
//using the webcam
void onMouse(int event, int x, int y, int, void*);
void selectColor(int event, int x, int y, int , void*);
void setMinMaxColor(Vec3 target, Vec3& minColor, Vec3& maxColor, Vec3 threshold);

int main()
{

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

    cv::namedWindow("Display Image");
    cv::setMouseCallback("Display Image", selectColor);

    cv::namedWindow("Threshold img");
    cv::moveWindow("Threshold img", 0, 0);
    //cv::namedWindow("red img");
    //cv::moveWindow("red img", 50, 450);
    //usleep(1000);

    while(running == true)
    {
        cv::Mat img;
        camera >> img;

        cv::Mat tmpImg;


        //// Read Camera Parameters //////
        cv::Mat camera_matrix, distortion;
        cv::FileStorage fs("fisheye0.txt", cv::FileStorage::READ);
        cv::FileNode fn = fs["IntParam"];
        fn["camera_matrix"] >> camera_matrix;
        fn["distortion"] >> distortion;

        //CvMat kMatrix = cvMat( 3, 3, CV_32FC1, kArray);

        //cv::Mat dMatrix = cvMat( 3, 3, CV_32FC1, dArray);

        //cv::fisheye::undistortImage(img, tmpImg, camera_matrix, distortion, camera_matrix);
        cv::undistort(img, tmpImg, camera_matrix, distortion);

        img = tmpImg;
        
        clock_t startTime = clock();

        //Colortracking
        ColorTracker ct;
        ct.setImage(img);
        ct.generateBinary(minColor, maxColor, true);

        //ColorTracker rt;
        //rt.setImage(img);
        //rt.generateBinary(minRed, maxRed, true);


        //Getting the blob data from the trackers
        ct.generateBlobs();
        std::deque< Flooder::Blob > blobs = ct.getBlobs(60);

        int maxBlobSize = 0;
        for(unsigned int i = 0; i < blobs.size(); i++)
        {
            if(blobs.at(i).pixelAmount > maxBlobSize)
            {
                maxBlobSize = blobs.at(i).pixelAmount;
            }
        }
        std::cout << "The biggset blob is: " << maxBlobSize << std::endl;

        for(unsigned int i = 0; i < blobs.size(); i++)
        {
            ct.drawCircle(blobs.at(i).center, 10, cv::Scalar(255, 0, 0));
        }
        
        clock_t endTime = clock();

        std::cout << "Search took: " << (float)(endTime-startTime)/CLOCKS_PER_SEC << std::endl;
        std::cout << "Found: " << blobs.size() << " blobs" << std::endl;

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
    setMinMaxColor(redColor, minColor, maxColor, threshold);
}


void setMinMaxColor(Vec3 target, Vec3& minColor, Vec3& maxColor, Vec3 threshold)
{
    minColor = target - threshold;
    maxColor = target + threshold;
}
