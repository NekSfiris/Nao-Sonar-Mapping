/*
 * Started 19/11/17
 * Author : Nek
 */

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

int main(int argc, char **argv)
{
  //Initiate new ROS node named talker
  ros::init(argc, argv, "talker");


  //create a node handle: it is reference assigned to a new node
  ros::NodeHandle n;

  /*Create a publisher with a topic 'chatter' that will send a String message
  The second parameter to advertise() is the size of the message queue
  used for publishing messages.  If messages are published more quickly
  than we can send them, the number here specifies how many messages to
  buffer up before throwing some away.
  Last two lines create a publisher object that can talk to ROS
  Tell the master that we are going to be publishing a message of
  type std_msgs/String on the topic chatter.*/
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

  //Rate is a class that is used to define frequency for a loop
  //Here we send a message each 1 seconds
  ros::Rate loop_rate(1.0);//1 message per second


  
  int count = 0;
  while (ros::ok())//Keep spinning loop until user presses Ctrl+C
  {

    //create a new String ROS message
    std_msgs::String msg;

    //create a string for the data
    std::stringstream ss;
    ss << "hello world " << count;

    //assign the string data to ROS message data field
    msg.data = ss.str();

    //print the content of the message in the terminal
    ROS_INFO("[Talker] I published %s\n", msg.data.c_str());

    //publish the message
    chatter_pub.publish(msg);

    //Need to call this function often to allow ROS to process incoming messages
    ros::spinOnce();


    //Sleep for the rest of the cycle, to enforce the loop rate
    loop_rate.sleep();

    count++;
  }


  return 0;
}


