#include "Tracker.h"

void Tracker::setup() 
{
    cv::namedWindow("GUI");
    cv::setMouseCallback("GUI", Tracker::onMouseGUI);
    cv::moveWindow("GUI", 0, 500);
    
    cv::namedWindow("Camera");
    cv::moveWindow("Camera", 500, 250);
}

void Tracker::loop() 
{
    //Capture an image
    captureNewImage();

    cv::imshow("Camera", currentImage);

    ct.setImage(currentImage);

    ct.runTracker();

    //Update UI stuff
    cv::waitKey(1);
}

/*
 * returns true if the program should exit, false otherwise
 */
bool Tracker::isDone() 
{
    return done;
}


//////////////////////////////////////////////////////////////////////////
//                      Private members
//////////////////////////////////////////////////////////////////////////
void Tracker::captureNewImage()
{
#ifndef USE_VIDEO
    camera >> currentImage;
#endif
}

/*
 * Event handler for when the 'GUI' window is clicked
 */
void Tracker::onMouseGUI(int event, int x, int y, int a, void* this_)
{
    if(event == 1)
    {
        ((Tracker*)this_)->done = true;
    }
}


/*
 * Event handler for when the camera window is clicked
 */
void Tracker::onMouseCamera(int event, int x, int y, int a, void* this_)
{
    if(event == 1)
    {
        ((Tracker*)this_)->done = true;
    }
}
