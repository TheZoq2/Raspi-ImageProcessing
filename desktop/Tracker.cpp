#include "Tracker.h"

void Tracker::setup() 
{
    cv::namedWindow("GUI");
    cv::setMouseCallback("GUI", Tracker::onMouseGUI);
    cv::moveWindow("GUI", 0, 500);
    
    cv::namedWindow("Camera");
    cv::moveWindow("Camera", 500, 250);

    //camera.set(CV_CAP_PROP_BRIGHTNESS , 100);
}

void Tracker::loop() 
{
    //Capture an image
    captureNewImage();

    cv::imshow("Camera", currentImage);

    ct.setImage(currentImage);

    ct.runTracker();
    cv::imshow("Binary", ct.getBinary());
    
    std::vector< Flooder::Blob > foundBlobs = ct.getBlobs(150);

    std::cout << foundBlobs.size() << std::endl;
    for(auto blob : foundBlobs) 
    {
        std::cout<< blob.center.getString() << std::endl;
    }
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
