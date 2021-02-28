#include <ros/ros.h>
#include "main2021/Data.h"
#include "main2021/dataToBig.h"
#include "main2021/dataToSmall.h"
#include "main2021/cup_camera.h"

#include "../include/main2021/data_state.h"

#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(int argc, char** argv){
    ros::init(argc, argv, "data_node");
    ros::NodeHandle nh;

	ros::Publisher pub_dataBig = nh.advertise<main2021::dataToBig>("DataToBig", 1000);
	ros::Publisher pub_dataSmall = nh.advertise<main2021::dataToSmall>("DataToSmall", 1000);
	
	main2021::dataToBig big;
	main2021::dataToSmall small;
	ros::Time begin_time;
	ros::Time now_time;

	data_state state;
	ros::Subscriber sub_data = nh.subscribe<main2021::Data>("giveToData", 1000, &data_state::datacallback, &state);

	int now_status = 0;
	bool firstrun = false;
	bool cameraCall = false;
	float doing_time = 0;

	if(ros::ok() == false)
		ROS_INFO("Data FALSE");
	while(ros::ok()){
		//To Do******
		//ROS_INFO("Data: %d", state.now_status());
		switch(state.now_status()){
			case 0://戰術選擇
				now_status = 1;
				ROS_INFO("SWITCH 1");
				cameraCall = false;
				break;
			case 1://機構,底盤reset
				now_status = 2;
				ROS_INFO("SWITCH 2");
				cameraCall = false;
				break;
			case 2://更新Data
				now_status = 3;
				ROS_INFO("SWITCH 3");
				cameraCall = true;
				break;
			case 3://拿另一機的資料
				now_status = 4;
				ROS_INFO("SWITCH 4");
				cameraCall = false;
				break;
			case 4://等拔插銷
				now_status = 5;
				ROS_INFO("SWITCH 5");
				cameraCall = false;
				break;
			case 5://run
				if(firstrun == false){
					begin_time = ros::Time::now();
					firstrun = true;
				}
				now_time = ros::Time::now();
				doing_time = (now_time - begin_time).toSec();
				cameraCall = true;
				// ROS_INFO("TIME:%f", doing_time);
				break;      
		}
		//ROS_INFO("NOWSTATUS:%d", now_status);
		state.unityAction();
		state.callCamera(cameraCall);
		//give big chicken
		big.small_x = state.get_sx();
		big.small_y = state.get_sy();
		big.small_degree = state.get_sdegree();
		big.small_action.assign(state.get_saction().begin(), state.get_saction().end());
		big.action_list.assign(state.get_list().begin(), state.get_list().end());
		big.cup_color.assign(state.get_color().begin(), state.get_color().end());
		big.cup = state.get_cup();
		big.ns = state.get_ns();
		big.team = state.get_team();
		big.time = doing_time;
		big.status = now_status;

		pub_dataBig.publish(big);
		//ROS_INFO("BIG");
		//give small chicken
		small.big_x = state.get_bx();
		small.big_y = state.get_by();
		small.big_degree = state.get_bdegree();
		small.big_action.assign(state.get_baction().begin(), state.get_baction().end());
		small.action_list.assign(state.get_list().begin(), state.get_list().end());
		small.cup_color.assign(state.get_color().begin(), state.get_color().end());
		small.cup = state.get_cup();
		small.ns = state.get_ns();
		small.team = state.get_team();
		small.time = doing_time;
		small.status = now_status;

		pub_dataSmall.publish(small);
		ros::spinOnce();
		//state.spinonce();
	}

	return 0;
}
