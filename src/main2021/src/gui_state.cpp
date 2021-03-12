#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include "main2021/gui_state.h"

#include <iostream>
#include <stdlib.h>

GUI::GUI(){

	//publish to gui
	status_pub = n.advertise<std_msgs::Int32>("pub_status", 10);
	strategy_sub = n.subscribe<std_msgs::Int32>("update_status", 10, &GUI::strategy_sub_callback,this);    

    change_sta = 0;
}

void GUI::pubToGUI(int state){
    pub_gui.data = state;
    
    status_pub.publish(pub_gui);
}

void GUI::strategy_sub_callback(const std_msgs::Int32::ConstPtr& msg){
    change_sta = msg->data;
}

int GUI::changState(){
    ros::spinOnce();

    return change_sta;
}