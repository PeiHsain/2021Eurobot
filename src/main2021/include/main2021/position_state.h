#ifndef POSITION_STATE_H_
#define POSITION_STATE_H_

#include <ros/ros.h>
#include "main2021/plannerState.h"

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

class Position{
public:
    Position();

    void give_plan(float, float, float);
    void P_callback(const main2021::plannerState::ConstPtr&);
    void E1_callback(const geometry_msgs::PoseStamped::ConstPtr&);
    void E2_callback(const geometry_msgs::PoseStamped::ConstPtr&);
    void L_callback(const nav_msgs::Odometry::ConstPtr&);
    
    bool findWay();
    int get_p_state();
    float get_e1_x();
    float get_e1_y();
    float get_e2_x();
    float get_e2_y();
    float get_px();
    float get_py();
    float get_pz();
    float getdegree();
  

private:
    ros::NodeHandle n;
    ros::Subscriber sub_planner;
    ros::Publisher pub_planner;
    ros::Subscriber sub_enemy1;
    ros::Subscriber sub_enemy2;
    ros::Subscriber sub_location;

    tf2::Quaternion odom_quat;
    geometry_msgs::PoseStamped transformStamped;

    float e1_x;
    float e1_y;
    float e2_x;
    float e2_y;
    float px;
    float py;
    float pz;

    double p_degree_R;
    double p_degree_P;
    double p_degree_Y;
    //[0] = 0:unarrived, 1:arrived, 2:emergency; [1] = 1:cannot find
    std::vector<int> p_state;
    
};

#endif
