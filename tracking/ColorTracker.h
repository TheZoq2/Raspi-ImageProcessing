#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

#include "Vec3.h"
#include "Vec2.h"
#include "ImgFunc.h" 
#include "Flooder.h"

#ifndef H_COLORTRACKER
#define H_COLORTRACKER

class ColorTracker
{
public:
    ColorTracker();

    //void piGrabImage(raspicam::RaspiCam_Cv* camera);
    void setImage(cv::Mat img);
    
    Vec3 getObjectColor(); //Select the color in the center of the image
    void generateBinary(Vec3 minThresh, Vec3 maxThresh, bool calcMiddle, bool calcSectors = false);
    void convertToHSV();

    void saveImage(std::string filename);
    void saveBinary(std::string filename);
    void saveHSV(std::string filename);

    void generateBlobs();

    Vec2 getMiddlePos();

    cv::Mat getBinary();
    std::deque< Flooder::Blob > getBlobs(int minSize);
private:
    cv::Mat lastImg;
    cv::Mat hsvImg;
    cv::Mat binaryImg; 
    
    std::vector< std::vector< int > > binaryMap;

    bool isConverted;

    int imgRows;
    int imgCols;
    
    Vec2 middlePos;


    int sectorAmount;
    int** sectors;
    bool hasSectors;

    std::deque< Flooder::Blob > blobs;
};
#endif
