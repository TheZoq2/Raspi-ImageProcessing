#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string>
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>

using namespace std; 

int frameWidth = 320;
int frameHeight = 240;

int threshold = 30;
cv::Scalar thresholdS(20, 40, 40);
cv::Scalar minColor;
cv::Scalar maxColor;

int clamp(int value, int min, int max)
{
    if(value < min)
    {
        value = min;
    }
    if(value > max)
    {
        value = max;
    }

    return value;
}

//Return the color of a pixel
uint8_t* getPixel(uint8_t* data, int x, int y, int rows, int cols, int channels)
{
    uint8_t* result;

    if(x > rows || y > cols)
    {
        std::cout << "failed to get pixel x: " << x << " > " << rows << " or ";
        std::cout << y << " > " << cols << std::endl;
        
        return 0;
    }

    //std::cout << "checking pixels" << std::endl;
    //Finding the pixel
    int rowIndex = (y-1) * cols * channels;
    int pixelStart = rowIndex + x * channels;

    //std::cout << "Pixel start: " << pixelStart << " rowIndex: " << rowIndex << " y: " << y << std::endl;
    
    for(int i = 0; i < channels; i++)
    {
        result[i] = data[pixelStart + i];
    }

    //std::cout << "Returning result" << std::endl;

    //std::cout << "Returning: [" << (int) result[0] << "," << (int) result[1] << "," << (int) result[2] << std::endl;
    
    return result;
}

//Return a 2d array containing the xy cordinates of a pixel from a data array
//of an image with rows rows and cols cols
int* getCoordsFromPixel(int pixel, int cols, int rows)
{
    int* result;
    result[0] = (pixel % cols) + 1;
    result[1] = pixel - floor(pixel / cols);

    return result;   
}
 
int main ( int argc,char **argv ) 
{
     
    time_t timer_begin,timer_end;
    raspicam::RaspiCam_Cv Camera;
    cv::Mat image;
    int nCount=100;
    //set camera params
    Camera.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
    Camera.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);
    //  Camera.set(CV_CAP_PROP_EXPOSURE, 50);
    
    //Open camera
    cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
    //Start capture
    cout<<"Capturing "<<nCount<<" frames ...."<<endl;
    time ( &timer_begin );
    
      
    //capture a few frames to adjust
    std::cout << "Precapturing" << std::endl;
    for(unsigned int i = 0; i < 10; i++)
    {
        Camera.grab();
    }
    
    bool useDefaultColor = false;
    std::cout << "Press enter to select an object" << std::endl;
    while(true)
    {
        if(getchar() == '\n')
            break;
        if(getchar() == 'd')
            useDefaultColor = true;
            break;
    }
    //Grab image to select object
    Camera.grab(); 
    Camera.retrieve(image); //Converting to HSV cv::Mat refHSV;
    
    cv::Mat refHSV;
    cv::cvtColor(image, refHSV, CV_BGR2HSV);
    //Calculating the center point of the pixel
    
    cv::imwrite("/home/frans/public_html/cvRefRaw.jpg", image);
    
    uint8_t* pixelData = (uint8_t*) refHSV.data;
    int pixelAmount = refHSV.rows * refHSV.cols;
    int middle = pixelAmount / 2;
    int pixelStart = middle * 3;
    int cH = pixelData[pixelStart + 0];
    int cS = pixelData[pixelStart + 1];
    int cV = pixelData[pixelStart + 2];
    
    int colorSum[3] = {0, 0, 0};
    //getting the average of the 10x10 center pixels
    int avgArea = 5;
    for(int x = -avgArea; x <= avgArea; x++)
    {
        for(int y = -avgArea; y <= avgArea; y++)
        {
            uint8_t* pixel = getPixel(pixelData, x + refHSV.rows/2, y + refHSV.cols / 2, refHSV.rows, refHSV.cols, 3);
            
            for(int i = 0; i < 3; i++)
            {
                colorSum[i] += pixel[i];
            }

            //delete[] pixel;
        }       
    }
    
    int avgAmount = pow(avgArea * 2 + 1, 2); //+1 because it 's area -area to area inclusive
    cH = colorSum[0] / avgAmount;
    cS = colorSum[1] / avgAmount;
    cV = colorSum[2] / avgAmount;
    /*cv::Vec3b centerPixel = image.at<cv::Vec3b>(image.rows / 2, image.cols / 2);
    cH = centerPixel.val[0];
    cS = centerPixel.val[1];
    cV = centerPixel.val[2];*/
    
    
    //Storing the threshold data
    int minNonMod[3];
    int maxNonMod[3];
    minNonMod[0] = cH - thresholdS.val[0];
    minNonMod[1] = cS - thresholdS.val[1];
    minNonMod[2] = cV - thresholdS.val[2];
    maxNonMod[0] = cH + thresholdS.val[0];
    maxNonMod[1] = cS + thresholdS.val[1];
    maxNonMod[2] = cV + thresholdS.val[2];
    
    //Making sure no overflow is taking place
    for(unsigned int i = 0; i < 3; i++)
    {
        minNonMod[i] = clamp(minNonMod[i], 0, 255);
        maxNonMod[i] = clamp(maxNonMod[i], 0, 255);  
        
        minColor.val[i] = minNonMod[i];
        maxColor.val[i] = maxNonMod[i];
    }
    
    std::cout << "Searching for [" << cH << "," << cS << "," << cV << "]" << " - Threshold: " << threshold << std::endl;
    std::cout << "maxColor: [" << maxColor.val[0] << "," << maxColor.val[1] << "," << maxColor.val[2] << "]";
    
    //Wait for enter again  
    std::cout << "Press enter to start search" << std::endl;
    
    while(true)
    {
        if(getchar() == '\n')
            break;
    } 
    
    Camera.grab();
    Camera.retrieve ( image);
    
    std::cout << "Done capturing" << std::endl;
    
    
    //processing the image
    //std::time(&timer_begin);
    std::clock_t timeStart = std::clock();
    
    //Image for storing HSV version
    cv::Mat imgHSV;
    cv::cvtColor(image, imgHSV, CV_BGR2HSV);
    
    //getitng the center pixel
    
    pixelData = (uint8_t*) imgHSV.data;
    pixelAmount = imgHSV.rows * imgHSV.cols;
    middle = pixelAmount / 2;
    pixelStart = middle * 3;
    cH = pixelData[pixelStart + 0];
    cS = pixelData[pixelStart + 1];
    cV = pixelData[pixelStart + 2];
    
    cv::Scalar cPixel(cH, cS, cV);
    std::cout << "Center pixel: [" << cH << "," << cS << "," << cV << "]" << std::endl;
    
/*    
    cv::Mat thrImg;
    //Perform thresholding
    //cv::inRange(imgHSV, minColor, maxColor, thrImg);
    //Converting into 3 planes
    
    cv::Mat planes[3];
    cv::Mat thrPlanes[3];
    cv::split(imgHSV, planes);
    for(int i = 0; i < 3; i++)
    {
        cv::Mat imgThresh;
        cv::Mat imgThreshHigh;
        cv::threshold(planes[i], imgThresh, minColor.val[i], 255, CV_THRESH_BINARY);
        cv::threshold(planes[i], imgThreshHigh, maxColor.val[i], 255, CV_THRESH_BINARY);
        cv::bitwise_and(imgThresh, imgThreshHigh, thrPlanes[i]);
    }
    cv::Mat imgResult(cv::Size(frameWidth, frameHeight), CV_8UC1, cv::Scalar(255));
    for(int i = 0; i < 3; i++)
    {
      cv::bitwise_and(imgResult, thrPlanes[i], imgResult);
    }
*/
    
    int xSum = 0; //Sum of all points on the x axis
    int ySum = 0; //Sum of all points on the y axis
    int inBoundAmount = 0;
    //Creating an image to store the stuff in
    cv::Mat imgResult(cv::Size(frameWidth, frameHeight), CV_8UC1, cv::Scalar(0));
    uint8_t* resultData = (uint8_t*) imgResult.data;
    
    for(int i = 0; i < frameWidth * frameHeight; i++)
    {
        bool inBounds = true;
        for(int n = 0; n < 3; n++)
        {
            if(pixelData[i * 3 + n] > maxColor.val[n] || pixelData[i * 3 + n] <  minColor.val[n])
            {

                inBounds = false;
            }
        }
        if(inBounds == true)
        {
            resultData[i] = 255;
            
            //adding to the sums
            inBoundAmount++;

            //Calculating the x and y coords of the pixel
        }
    }
    
    
    //std::time(&timer_end);
    std::clock_t timeEnd = std::clock();
    std::cout << "Processed image in :" << (timeEnd - timeStart ) / (double)(CLOCKS_PER_SEC/ 1000) << std::endl;
    
    cout<<"Stop camera..."<<endl;
    Camera.release();
    //show time statistics
    time ( &timer_end ); /* get current time; same as: timer = time(NULL)  */
    //save image 
    cv::imwrite("/home/frans/public_html/cvRaw.jpg", image);
    cv::imwrite("/home/frans/public_html/cvHSV.jpg", imgHSV);
    //cv::imwrite("/home/frans/public_html/cvThr.jpg", thrImg);
    cv::imwrite("/home/frans/public_html/cvThr.jpg", imgResult);
    cv::imwrite("/home/frans/public_html/cvRef.jpg", refHSV);
}
