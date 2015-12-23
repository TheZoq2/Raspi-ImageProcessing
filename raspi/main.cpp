#include <iostream>
#include <ctime>
#include <string>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include <stdio.h>

#include "objectTracking/Vec3.h"
#include "objectTracking/ColorTracker.h"
#include "objectTracking/ImgFunc.h"

#define END_LOOP_BUTTON 24

int frameWidth = 320;
int frameHeight = 240;

Vec3 objectColor;
Vec3 minColor;
Vec3 maxColor;
Vec3 threshold(20, 40, 40);

void clear()
{
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

int main()
{
    //cv::namedWindow("abc", CV_WINDOW_OPENGL);

    //Setting up the camera
    raspicam::RaspiCam_Cv camera;

    camera.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);

    std::cout << "Opening camera" << std::endl;
    if(!camera.open())
    {
        std::cout << "Error opening camera" << std::endl;
        return -1;
    }

    //Capturing a few frames to adjust the exposure of the camera
    std::cout << "Precapturing" << std::endl;
    cv::Mat testImg;
    for(unsigned int i = 0; i < 10; i++)
    {
        camera.grab();

        camera.retrieve(testImg);
    }
    std::cout << "Writing test img" << std::endl;
    cv::imwrite("/home/frans/public_html/test2.jpg", testImg);


    //Main menu
    bool running = true;
    while(running == true)
    {
        std::string input;
        
        std::cout << "1: select object" << std::endl;
        std::cout << "2: search once" << std::endl; 
        std::cout << "3: search" << std::endl; 
        std::cout << "0: exit" << std::endl;

        //Getting commands from the user
        std::cout << "Command: " << std::endl;
        std::cin >> input;
        
        //Checking the input
        if(input.compare("1") == 0)
        {
            //Grabbing an image
            cv::Mat image;
            
            camera.grab();

            std::cout << "Grabed image "<<std::endl;
            camera.retrieve(image);
            std::cout << "Retrieved image" << std::endl;
            
            //Processing the image
            ColorTracker ct;
            ct.setImage(image);
            ct.saveImage("/home/frans/public_html/cvRefRaw.jpg");

            objectColor = ct.getObjectColor();

            for(int i = 0; i < 3; i++)
            {
                minColor.val[i] = objectColor.val[i] - threshold.val[i];
                maxColor.val[i] = objectColor.val[i] + threshold.val[i];
            }

            //Print the color of the image
            std::cout << "Selected color: " << objectColor.getString() << std::endl;
        }
        else if(input.compare("2") == 0)
        {
            cv::Mat image;

            camera.grab();
            camera.retrieve(image);

            ColorTracker ct;
            ct.setImage(image);
            ct.generateBinary(minColor, maxColor, true);

            ct.saveBinary("/home/frans/public_html/cvThr.jpg");
            
            //Print the result of the search
            std::cout << "Found object in: " << ct.getMiddlePos().getString() << std::endl;

            
        }
        else if(input.compare("3") == 0)
        {
            //Color tracker object for tracking objects
            ColorTracker ct;
            cv::Mat image;
            cv::Mat pathImg(cv::Size(frameWidth, frameHeight), CV_8UC1,  cv::Scalar(0));

            bool capturing = true;

            std::clock_t searchStart = std::clock();
            while(capturing == true)
            {
                clear();

                //Grabbing an image
                camera.grab();
                camera.retrieve(image);

                ct.setImage(image);
                ct.generateBinary(minColor, maxColor, true);

                cv::imshow("abc", ct.getBinary());
                cv::waitKey(10);


                std::cout << "Found object in: " << ct.getMiddlePos().getString() << std::endl;
                std::cout <<  "Showing image" << std::endl;
                //storing the result in an image
                uint8_t* pathData = pathImg.data;
           
                int pixelPos = ImgFunc::getPixelStart(ct.getMiddlePos().val[0], ct.getMiddlePos().val[1], frameWidth, frameHeight, 1);

                if(pixelPos > 0 && pixelPos < frameWidth*frameHeight)
                {
                    pathData[pixelPos] = 255;
                    std::cout << "Writing pixel" << std::endl;
                }

                if((std::clock() - searchStart) / CLOCKS_PER_SEC >  3)
                {
                    capturing = false;

                    imwrite("/home/frans/public_html/path.jpg", pathImg);
                }
            }
        }
        else if(input.compare("0") == 0)
        {
           running = false;
        }
    }
    return 0;
}

