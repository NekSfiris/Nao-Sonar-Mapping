#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char **argv)
{
	//ROS node definition
        ros::init(argc, argv, "speek");
        ros::NodeHandle n;
        ros::Publisher speech_pub = n.advertise<std_msgs::String>("speech", 100);
        std_msgs::String str;
        str.data = "hello world";
        speech_pub.publish(str);
	
	ros::spin();

        return 0;
}
