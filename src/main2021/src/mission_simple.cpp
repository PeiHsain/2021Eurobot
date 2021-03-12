#include "ros/ros.h"
#include <ros/package.h> //not sure if needed
// #include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Float32MultiArray.h>
#include<geometry_msgs/PoseStamped.h>
#include "main2021/maintomission.h"

#include <sstream>
#include <stdio.h>
#include <iostream>
#include <string>
#include<queue>
#include<vector>
using namespace std;

int state_mission = 0;

void chatterCallback(const main2021::maintomission::ConstPtr& msg)
{
    state_mission = 1;
}

int main(int argc, char **argv)
{
 
ros::init(argc, argv, "mission");
ros::NodeHandle n;

ros::Publisher tomain = n.advertise<std_msgs::Int32>("missionToMain", 1);
ros::Subscriber sub = n.subscribe("mainToMission", 1, chatterCallback);

ros::Rate loop_rate(10);


  while (ros::ok())
  {
    std_msgs::Int32 to_main;

    to_main.data =  state_mission;
    tomain.publish(to_main);

    ros::spinOnce();

    loop_rate.sleep();

  }


  return 0;
}