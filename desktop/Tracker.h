#ifndef H_TRACKER
#define H_TRACKER

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <unistd.h>
#include <memory>

#include "config.h"

#include "objectTracking/ColorTracker.h"
#include "objectTracking/Flooder.h"
#include "Performance.h"


class Tracker
{
public:
    void setup();
    void loop();



    bool isDone();
private:
    static void onMouseGUI(int event, int x, int y, int, void* this_);
    static void onMouseCamera(int event, int x, int y, int, void* this_);

    void captureNewImage();

    #ifndef USE_VIDEO
        cv::VideoCapture camera = cv::VideoCapture(Config::CAMERA_ID);
    #endif
    
    bool done = false;

    std::vector< Flooder::Blob > blobs;

    cv::Mat currentImage;
    ColorTracker ct;

    Performance performance;
};


#endif
