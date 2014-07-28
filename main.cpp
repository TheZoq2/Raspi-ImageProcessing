#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string>
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>

using namespace std; 

int frameWidth = 640;
int frameHeight = 480;

int threshold = 50;
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


 
int main ( int argc,char **argv ) {
 
  time_t timer_begin,timer_end;
  raspicam::RaspiCam_Cv Camera;
  cv::Mat image;
  int nCount=100;
  //set camera params
  Camera.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
  Camera.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);

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
  
  std::cout << "Press enter to select an object" << std::endl;
  while(true)
  {
    if(getchar() == '\n')
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
  int cH = pixelData[pixelStart + 1];
  int cS = pixelData[pixelStart + 2];
  int cV = pixelData[pixelStart + 3];

  cv::Vec3b centerPixel = image.at<cv::Vec3b>(image.rows / 2, image.cols / 2);
  cH = centerPixel.val[0];
  cS = centerPixel.val[1];
  cV = centerPixel.val[2];
  
  std::cout << "middle pixel: " << middle << std::endl;

  //Storing the threshold data
  int minNonMod[3];
  int maxNonMod[3];
  minNonMod[0] = cH - threshold;
  minNonMod[1] = cS - threshold;
  minNonMod[2] = cV - threshold;
  maxNonMod[0] = cH + threshold;
  maxNonMod[1] = cS + threshold;
  maxNonMod[2] = cV + threshold;
  
  //Making sure no overflow is taking place
  for(unsigned int i = 0; i < 3; i++)
  {
    minNonMod[i] = clamp(minNonMod[i], 0, 255);
    maxNonMod[i] = clamp(maxNonMod[i], 0, 255);  
    
    minColor.val[i] = minNonMod[i];
    maxColor.val[i] = maxNonMod[i];
  }

  std::cout << "Searching for [" << cH << "," << cS << "," << cV << "]" << " - Threshold: " << threshold << std::endl;

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
  std::time(&timer_begin);
  
  //Image for storing HSV version
  cv::Mat imgHSV;
  cv::cvtColor(image, imgHSV, CV_BGR2HSV);

  //getitng the center pixel

  pixelData = (uint8_t*) imgHSV.data;
  pixelAmount = imgHSV.rows * imgHSV.cols;
  middle = pixelAmount / 2;
  pixelStart = middle * 3;
  cH = pixelData[pixelStart + 1];
  cS = pixelData[pixelStart + 2];
  cV = pixelData[pixelStart + 3];

  cv::Scalar cPixel(cH, cS, cV);
  std::cout << "Center pixel: [" << cH << "," << cS << "," << cV << "]" << std::endl;
  
  cv::Mat thrImg;
  //Perform thresholding
  cv::inRange(imgHSV, minColor, maxColor, thrImg);

  std::time(&timer_end);
  std::cout << "Processed image in :" << (double)timer_end - (double)timer_begin << std::endl;

  cout<<"Stop camera..."<<endl;
  Camera.release();
  //show time statistics
  time ( &timer_end ); /* get current time; same as: timer = time(NULL)  */
  //save image 
  cv::imwrite("/home/frans/public_html/cvRaw.jpg", image);
  cv::imwrite("/home/frans/public_html/cvHSV.jpg", imgHSV);
  cv::imwrite("/home/frans/public_html/cvThr.jpg", thrImg);
  cv::imwrite("/home/frans/public_html/cvRef.jpg", refHSV);
}
