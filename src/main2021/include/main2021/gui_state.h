#ifndef GUI_STATE_H_
#define GUI_STATE_H_

#include <ros/ros.h>
#include <std_msgs/Int32.h>

#include <iostream>
#include <stdlib.h>

using namespace std;

class GUI{
public:
    GUI();
    void pubToGUI(int);
    void strategy_sub_callback(const std_msgs::Int32::ConstPtr&);
    int changState();

private:
    ros::NodeHandle n;
    ros::Publisher status_pub;
    std_msgs::Int32 pub_gui;
    ros::Subscriber strategy_sub;

    int change_sta;
};

#endif