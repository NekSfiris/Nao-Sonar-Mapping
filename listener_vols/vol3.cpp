/*
 * Started 19/11/17
 * Author : Nek
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

//http://wiki.ros.org/cv_bridge/Tutorials/ConvertingBetweenROSImagesAndOpenCVImagesPython
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <geometry_msgs/Twist.h>

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

int ch=0;//counter inside chatterCallback function
int c=0;//counter to see at which value of degrees we are
bool img_f=0;//flag to start image creation
bool st_f=1;//flag to start storing topic data
bool mov_f=0;//flag to start moving

float arr[12] = {};//array full of floats that are 0
float fin[12] = {};//final values for range for each set of degrees

double PI = 3.1415926535897;
double speed=10;
double angle=30;

//--------------------------------------------------------------------//
///////////////-----------------------------------------////////////////
//--------------------------------------------------------------------//

class SonarStarter
{
	//Create a node handler it is reference assigned to a new node
	ros::NodeHandle n;
	ros::Publisher chatter_pub;
	ros::Subscriber sub;

	public:
	  	SonarStarter()
	  	{

	        /*Create a publisher for a topic '/cmd_vel' that will send a geometry message
			The second parameter to advertise() is the size of the message queue
			used for publishing messages.  If messages are published more quickly
			than we can send them, the number here specifies how many messages to
			buffer up before throwing some away.*/
	        chatter_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);

			//subscribe to a given topic, in this case: left sonar
			//chatterCallBack: is the name of the callback function that will be executed
			//each time a message is recieved
			sub = n.subscribe("/naoqi_driver/sonar/left", 1000, &SonarStarter::chatterCallback,this);

		}


		// Topic messages callback
		void chatterCallback(const sensor_msgs::Range::ConstPtr& msg)
		{

			ROS_INFO("Sonar Seq: [%d]", msg->header.seq);
			ROS_INFO("Sonar Range: [%f]\n", msg->range);

			if(st_f==1)//apothikeuse tis 12 teleutaies times apo to sonar
			{
				ch++;
				//petao tis 3 protes times epeidi einai poli pithano na periexoun
				//skoupidia pou den thelo (eidika meta tin kinisi)
				if(ch>3)
				{	
					arr[ch]=msg->range;
				}
				if(ch==15)
				{
					ch=0;
					st_f=0;
					mov_f=1;//afou tis apothikeuses stripse

					cout << "TOOK 12 VALUES!!!" << endl;

					float b=0.0;
					for(int a=0;a<12;a=a+1)
					{
						b=b+arr[a];
						//cout << "b=" << b << endl;
					}

					fin[c]=b/12;//mean float number for this value of degrees
					cout << "FINAL VALUE FOR THIS SET IS : " << fin[c] << endl<< endl;

					c++;//next value of degrees
				}
			}

			if(c==11)//time to create the grid map
			{
				//int z=ImageCreator();
			}

			if(mov_f==1)//if 1, we move to another value of degrees
			{
				cout << "TIME TO CALL MoveBase FUNCTION" << endl<< endl;
				MoveBase();
			}
		}


		void MoveBase()
		{

			double curr_angle=0;

			cout << "I'M INSIDE MoveBase" << endl << endl;

			geometry_msgs::Twist g_msg;
			g_msg.linear.x=0;
			g_msg.linear.y=0;
			g_msg.linear.z=0;
			g_msg.angular.x=0;
			g_msg.angular.y=0;

			double angular_speed = speed*2*PI/360;

			g_msg.angular.z=angular_speed;//degrees/sec to rad/sec

  			double relative_angle = angle*2*PI/360;//degrees to rad

  			ros::Time::init();
  			double t0 = ros::Time::now().toSec();
  			double t1;
  			//ROS_INFO("t0 = [%f]\n", t0);
  			
  			//trying to move little by little until we reach the angle we want
  			while(curr_angle<relative_angle)
  			{
  				chatter_pub.publish(g_msg);
  				t1 = ros::Time::now().toSec();
  				curr_angle=angular_speed*(t1-t0);
  			}

  			cout << "Speed in rads/sec = " << fixed << angular_speed << endl;
  			cout << "Angle in rads/sec = " << fixed << relative_angle << endl<< endl;
  			cout << "t0 = " << fixed << t0 << endl;
  			cout << "t1 = " << fixed << t1 << endl;
  			cout << "t1-t0 = " << fixed << t1-t0 << endl;

  			//now we force to stop moving
  			for(int i=0;i<10;i++)
  			{
  				g_msg.angular.z=0;
				chatter_pub.publish(g_msg);
  			}
			

			cout << "DONE THE MOVE!!!!!!!!!!" << endl << endl;

			//cout << "IT IS SUPPOSED TO SLEEP HERE!!!!!!!!!!" << endl;
			//we have to wait some time for torso stabilization

		/*	while(!ac.waitForServer(ros::Duration(5.0))){
			       ROS_INFO("Waiting for the move_base action server to come up");
			     }

			//here is a dummy while to make the node wait

			double t2 = ros::Time::now().toSec();
  			double t3;
  			curr_angle=0;
  			while(curr_angle<relative_angle)
  			{
  				t1 = ros::Time::now().toSec();
  				
  				curr_angle=(5*PI/360)*(t1-t0);
  			}
		*/
			//cout << "AFTER THE SLEEP!!!!!!!!!!" << endl << endl;

			//after the stabilization of the sonar range values
			//we are ready to take the new ones
			st_f=1;
			mov_f=0;
			
		}
};


int ImageCreator()
{
	//read image 600*600 2D grid map (x,y)
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

int main(int argc, char **argv)
{
	ros::init(argc, argv, "listener");// Initiate a new ROS node named listener
	SonarStarter sr;
	ros::spin();// Enter a loop, pumping callbacks
	return 0;
}



