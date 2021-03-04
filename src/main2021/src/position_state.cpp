#include <ros/ros.h>
#include "main2021/plannerState.h"

#include "../include/main2021/position_state.h"

//tf2 must
#include <tf2_ros/transform_broadcaster.h>
#include "tf2_ros/transform_listener.h"
#include <tf2/LinearMath/Quaternion.h>
#include "tf2/LinearMath/Transform.h"
#include <tf2/LinearMath/Scalar.h>
//not sure
#include "tf2/convert.h"
#include "tf2/utils.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/message_filter.h"
#include "message_filters/subscriber.h"

#include "nav_msgs/Odometry.h"

#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

Position::Position(){
    sub_planner = n.subscribe<main2021::plannerState>("plan_state", 1, &Position::P_callback, this);
    pub_planner = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);
    sub_enemy1 = n.subscribe<geometry_msgs::PoseStamped>("/enemy_pose", 1000, &Position::E1_callback, this);
    sub_enemy2 = n.subscribe<geometry_msgs::PoseStamped>("/enemy_pose2", 1000, &Position::E2_callback, this);
    sub_location = n.subscribe<nav_msgs::Odometry>("/global_filter", 1000, &Position::L_callback, this);

    e1_x = 0.;
    e1_y = 0.;
    e2_x = 0.;
    e2_y = 0.;
    px = 800.;
    py = 2700.;
    pz = 0.;
    p_degree_R = 0;
    p_degree_P = 0;
    p_degree_Y = 0;
    for(int i = 0 ; i < 2 ; i++)
        p_state.push_back(0);
}

void Position::give_plan(float action_x, float action_y, float action_th){
    transformStamped.header.stamp = ros::Time::now();
    transformStamped.header.frame_id = "map";
    //publish is meter
    transformStamped.pose.position.x = (action_x/1000);//meter
    transformStamped.pose.position.y = (action_y/1000);//meter
    transformStamped.pose.position.z = 0;
    odom_quat.setRPY(0, 0, action_th);
    transformStamped.pose.orientation.x = odom_quat.x();
    transformStamped.pose.orientation.y = odom_quat.y();
    transformStamped.pose.orientation.z = odom_quat.z();
    transformStamped.pose.orientation.w = odom_quat.w();

    pub_planner.publish(transformStamped);
}

void Position::P_callback(const main2021::plannerState::ConstPtr& msg){
    //p_state[0] = done or not, p_state[1] = cannot find
    p_state.assign(msg->plan_state.begin(), msg->plan_state.end());
}
void Position::E1_callback(const geometry_msgs::PoseStamped::ConstPtr& msg){
    //receive is meter
    e1_x = (msg->pose.position.x * 1000);
    e1_y = (msg->pose.position.y * 1000);
}
void Position::E2_callback(const geometry_msgs::PoseStamped::ConstPtr& msg){
    //receive is meter
    e2_x = (msg->pose.position.x * 1000);
    e2_y = (msg->pose.position.y * 1000);
}
void Position::L_callback(const nav_msgs::Odometry::ConstPtr& msg){
    //receive is meter
    px = (msg->pose.pose.position.x * 1000);
    py = (msg->pose.pose.position.y * 1000);
    pz = (msg->pose.pose.position.z * 1000);
    // ROS_INFO("position px: %f", px);
    // ROS_INFO("position py: %f", py);

    tf2::Quaternion q(msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
    tf2::Matrix3x3 m(q);
    m.getRPY(p_degree_P, p_degree_R, p_degree_Y);
    // ROS_INFO("position th: %f", p_degree_Y);  
}

//can plan?
bool Position::findWay(){
    if(p_state[1] == 1)
        return false;
    else
        return true;
}
int Position::get_p_state(){
    ros::spinOnce();
    ROS_INFO("Pcall");

    if(findWay())
        return p_state[0];
    else
        return 2;//--> cannot find
}
float Position::get_e1_x(){ return e1_x;}
float Position::get_e1_y(){ return e1_y;}
float Position::get_e2_x(){ return e2_x;}
float Position::get_e2_y(){ return e2_y;}
float Position::get_px(){ return px;}
float Position::get_py(){ return py;}
float Position::get_pz(){ return pz;}
float Position::getdegree(){ return (float)p_degree_Y;}

// void Position::rx(){
//     ros::spinOnce();
// }

