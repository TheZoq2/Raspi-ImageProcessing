#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

#include "Vec3.h"
#include "Vec2.h"
#include "ImgFunc.h" 
#include "Flooder.h"
#include "Array2d.h"

#ifndef H_COLORTRACKER
#define H_COLORTRACKER

class ColorTracker
{
public:
    ColorTracker();

    //void piGrabImage(raspicam::RaspiCam_Cv* camera);
    void setImage(cv::Mat img);
    
    Vec3 getObjectColor(); //Select the color in the center of the image
    Vec3 getColorInPixel(Vec2 pixel);
    void generateBinary(Vec3 minThresh, Vec3 maxThresh, bool calcMiddle);
    void convertToHSV();
    void drawCircle(Vec2 pos, float radius, cv::Scalar color);

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
    
    //std::vector< std::vector< int > > binaryMap;
    Array2d<bool> binaryMap;
    

    bool isConverted;

    std::size_t imgRows;
    std::size_t imgCols;
    
    Vec2 middlePos;


    int sectorAmount;
    int** sectors;
    bool hasSectors;

    std::deque< Flooder::Blob > blobs;
};
#endif
