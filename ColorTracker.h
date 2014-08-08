#include <iostream>
#include <string>
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>

#include "Vec3.h"
#include "Vec2.h"
#include "ImgFunc.h" 
#ifndef H_COLORTRACKER
#define H_COLORTRACKER

class ColorTracker
{
public:
    ColorTracker();

    //void piGrabImage(raspicam::RaspiCam_Cv* camera);
    void setImage(cv::Mat img);
    
    Vec3 getObjectColor(); //Select the color in the center of the image
    void generateBinary(Vec3 minThresh, Vec3 maxThresh, bool calcMiddle);
    void convertToHSV();

    void saveImage(std::string filename);
    void saveBinary(std::string filename);
    void saveHSV(std::string filename);

    Vec2 getMiddlePos();
private:
    cv::Mat lastImg;
    cv::Mat hsvImg;
    cv::Mat binaryImg; 

    bool isConverted;

    int imgRows;
    int imgCols;
    
    Vec2 middlePos;
};
#endif
