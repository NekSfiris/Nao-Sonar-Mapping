# Nao-Sonar-Mapping

A ROS based package implementation for Nao v5 robot to create a polar map of its surroundings using its sonars in C++.
Check code in order to change angle of rotation and motion speed.

### Outcome Example

![alt text](https://github.com/NekSfiris/Nao-Sonar-Mapping/blob/master/presentation%20Embedded/Image_screenshot_08.12.2017.png)

### Prerequisites

You need to have installed :
* ROS ( with a catkin environment )
* naoqi_driver package

Has been tested for ros-kinetic.

### Build

After you downloaded the package, move it in your catkin workspace and :
```
$ cd ~/catkin_ws
$ catkin_make
```

### Run

To run the package first run naoqi_driver in one terminal.
Then in another terminal : 
```
$ roslaunch sonar_package sonar_map.launch
```
