/*
 * Started 19/11/17
 * Author : Nek
 */

//These are for general c++ and opencv.
#include <stdio.h>
#include <iostream>

//http://wiki.ros.org/cv_bridge/Tutorials/ConvertingBetweenROSImagesAndOpenCVImagesPython
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/core.hpp>

//These are for ros.
//http://wiki.ros.org/Robots/evarobot/Tutorials/indigo/Sonar
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Range.h"

//All the OpenCV classes, functions, and new API are placed into the cv namespace.
//Therefore, to access this functionality from your code,
//use the cv:: specifier (cv::Mat H = cv::findHomography(p1,p2,CV_RANSAC,5))
//or using namespace cv
using namespace cv;
using namespace std;

bool im_c=0;

// Topic messages callback
void chatterCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  ROS_INFO("Sonar Seq: [%d]", msg->header.seq);
  ROS_INFO("Sonar Range: [%f]\n", msg->range);
}

int main(int argc, char **argv)
{

  // Initiate a new ROS node named listener
  ros::init(argc, argv, "listener");

  // Create a node handler it is reference assigned to a new node
  ros::NodeHandle n;

  ROS_INFO("\n!!!---!!!Started running!!!---!!!\n");

  //subscribe to a given topic, in this case: left sonar
  //chatterCallBack: is the name of the callback function that will be executed
  //each time a message is recieved
  ros::Subscriber sub = n.subscribe("/naoqi_driver/sonar/left", 1000, chatterCallback);
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

  if(im_c=1)
  {
	  //read image 600*600 grid map (x,y)
	  //half on horizontal axis x=300
	  //half on vertical axis y=300
	  Mat img = imread("/home/nektar/catkin_ws/src/sonar_package/map_images/map3.jpg");

	  if (!img.data)//check if image loaded
	  {
	    std::cout << "Image not loaded\n";
	    return -1;
	  }

	  namedWindow( "Image", WINDOW_AUTOSIZE );// Create a window for display.
	  imshow("Image", img);
	  

	  
	  /*
	  Mat image = Mat::zeros( 400, 400, CV_8UC3 );
	  circle( image, Point( 200, 200 ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
	  imshow("Image",image);
	  */

	  waitKey();
	  return 0;
  }

  // Enter a loop, pumping callbacks
  ros::spin();

  return 0;
}



