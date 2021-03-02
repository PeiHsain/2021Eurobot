#include <ros/ros.h>
#include <std_msgs/String.h>
#include "main2021/Data.h"
#include "main2021/cup_camera.h"
#include "main2021/ns.h"
#include "main2021/cup.h"

#include "../include/main2021/data_state.h"


#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

data_state::data_state(){
	//ns--> 0:n 1:s
	client_ns = h.serviceClient<main2021::ns>("cup_service");
	//array--> 1:red 0:green
	client_cup = h.serviceClient<main2021::cup>("ns_service");
	client_camera = h.serviceClient<main2021::cup_camera>("cup_camera");

	ns_srv.request.OAO = false;
	cup_srv.request.OUO = false;
	c_srv.request.req = false;

	sx = 0;
	sy = 0;
	sdegree = 0;
	saction = {0};
	saction_list = {0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0};
	scup = 65535;

	bx = 800;
	by = 200;
	bdegree = 0;
	baction = {0};
	baction_list = {0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0};
	bcup = 65535;

	action_list = {0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0};
	cup_color = {0, 0, 0, 0, 0};
	camera_cup_color = {0};
	camera_cup_pos = {0};
	cup = 65535;
	ns = 0;
	team = 0;

	//spinonce();
	initial_cup_pos();
}
void data_state::initial_cup_pos(){
	//blue
	if(team == 0){
		//( num, (x, y), 1 for cup still there 0 for cup gone, 2 for green 3 for red)
		bc[0] = {.num = 1, .pos = pair<float, float>(1200., 300.), .color = 2, .state = true}; bc[1] = {.num = 2, .pos = pair<float, float>(1085., 445.), .color = 3, .state = true};
		bc[2] = {.num = 3, .pos = pair<float, float>(515., 445.), .color = 2, .state = true}; bc[3] = {.num = 4,.pos = pair<float, float>(400., 300.), .color = 3, .state = true};
		bc[4] = {.num = 5, .pos = pair<float, float>(100., 670.), .color = 2, .state = true}; bc[5] = {.num = 6, .pos = pair<float, float>(400., 956.), .color = 3, .state = true}; 
		bc[6] = {.num = 7, .pos = pair<float, float>(800., 1100.), .color = 2, .state = true}; bc[7] = {.num = 8, .pos = pair<float, float>(1200., 1270.), .color = 3, .state = true};
		bc[8] = {.num = 9, .pos = pair<float, float>(1200., 1730.), .color = 2, .state = true}; bc[9] = {.num = 10, .pos = pair<float, float>(800., 1900.), .color = 3, .state = true};
		bc[10] = {.num = 11, .pos = pair<float, float>(400., 2044.), .color = 2, .state = true}; bc[11] = {.num = 12, .pos = pair<float, float>(100., 2330.), .color = 3, .state = true};
		bc[12] = {.num = 13, .pos = pair<float, float>(1655., 1665.), .color = 2, .state = false}; bc[13] = {.num = 14, .pos = pair<float, float>(1655., 1935.), .color = 3, .state = false};
		bc[14] = {.num = 15, .pos = pair<float, float>(1955., 1605.), .color = 3, .state = false}; bc[15] = {.num = 16, .pos = pair<float, float>(1955., 1995.), .color = 2, .state = false};
		bc[16] = {.num = 17, .pos = pair<float, float>(0., 0.), .color = 3, .state = false}; bc[17] = {.num = 18, .pos = pair<float, float>(0., 0.), .color = 3, .state = false};
		bc[18] = {.num = 19, .pos = pair<float, float>(0., 0.), .color = 3, .state = false}; bc[19] = {.num = 20, .pos = pair<float, float>(0., 0.), .color = 3, .state = false};
		bc[20] = {.num = 21, .pos = pair<float, float>(0., 0.), .color = 3, .state = false}; bc[21] = {.num = 22, .pos = pair<float, float>(0., 0.), .color = 3, .state = false};
		bc[22] = {.num = 23, .pos = pair<float, float>(0., 0.), .color = 3, .state = false}; bc[23] = {.num = 24, .pos = pair<float, float>(0., 0.), .color = 3, .state = false};

	}
	//yellow
	else{
		bc[0] = {.num = 1, .pos = pair<float, float>(1200., 2700.), .color = 2, .state = true}; bc[1] = {.num = 2, .pos = pair<float, float>(1085., 2555.), .color = 3, .state = true};
		bc[2] = {.num = 3, .pos = pair<float, float>(515., 2555.), .color = 2, .state = true}; bc[3] = {.num = 4,.pos = pair<float, float>(400., 2700.), .color = 3, .state = true};
		bc[4] = {.num = 5, .pos = pair<float, float>(100., 670.), .color = 2, .state = true}; bc[5] = {.num = 6, .pos = pair<float, float>(400., 956.), .color = 3, .state = true};
		bc[6] = {.num = 7, .pos = pair<float, float>(800., 1100.), .color = 2, .state = true}; bc[7] = {.num = 8, .pos = pair<float, float>(1200., 1270.), .color = 3, .state = true};
		bc[8] = {.num = 9, .pos = pair<float, float>(1200., 1730.), .color = 2, .state = true}; bc[9] = {.num = 10, .pos = pair<float, float>(800., 1900.), .color = 3, .state = true};
		bc[10] = {.num = 11, .pos = pair<float, float>(400., 2044.), .color = 2, .state = true}; bc[11] = {.num = 12, .pos = pair<float, float>(100., 2330.), .color = 3, .state = true};
		bc[12] = {.num = 13, .pos = pair<float, float>(1655., 1065.), .color = 2, .state = false}; bc[13] = {.num = 14, .pos = pair<float, float>(1655., 1335.), .color = 3, .state = false};
		bc[14] = {.num = 15, .pos = pair<float, float>(1955., 1005.), .color = 3, .state = false}; bc[15] = {.num = 16, .pos = pair<float, float>(1955., 1395.), .color = 2, .state = false};
		bc[16] = {.num = 17, .pos = pair<float, float>(0., 0.), .color = 3, .state = false}; bc[17] = {.num = 18, .pos = pair<float, float>(0., 0.), .color = 3, .state = false};
		bc[18] = {.num = 19, .pos = pair<float, float>(0., 0.), .color = 3, .state = false}; bc[19] = {.num = 20, .pos = pair<float, float>(0., 0.), .color = 3, .state = false};
		bc[20] = {.num = 21, .pos = pair<float, float>(0., 0.), .color = 3, .state = false}; bc[21] = {.num = 22, .pos = pair<float, float>(0., 0.), .color = 3, .state = false};
		bc[22] = {.num = 23, .pos = pair<float, float>(0., 0.), .color = 3, .state = false}; bc[23] = {.num = 24, .pos = pair<float, float>(0., 0.), .color = 3, .state = false};
	}
}
void data_state::callNS(int req){
	ns_srv.request.OAO = req;
	int i = 0;
	while (i == 0)
	{
		if(client_ns.call(ns_srv)){
			ROS_INFO("GET NS");
			ns = ns_srv.response.ns;
			i = 1;			
		}
	}
	
}
void data_state::callCup(int req){
	cup_srv.request.OUO = req;
	int i = 0;
	while (i == 0)
	{
		if(client_cup.call(cup_srv)){
			// ROS_INFO("GET NS");
			cup_color.assign(cup_srv.response.CupResult.begin(), cup_srv.response.CupResult.end());
			i = 1;			
		}
	}
}
void data_state::callCamera(bool req){
	 c_srv.request.req = req;

	int i = 0;
	while(i == 0){
		// ROS_INFO("call");
		if(client_camera.call(c_srv)){
			// ROS_INFO("CUP");
			camera_cup_color.assign(c_srv.response.color.begin(), c_srv.response.color.end());
			camera_cup_pos.assign(c_srv.response.cup_pos.begin(), c_srv.response.cup_pos.end());
			i = 1;			
		}
		else
			ROS_INFO("fail call");
	}
	
	unityCup();
}

float data_state::get_sx(){ return sx;}
float data_state::get_sy(){ return sy;}
float data_state::get_sdegree(){ return sdegree;}
vector<int>& data_state::get_saction(){ return saction;}

float data_state::get_bx(){ return bx;}
float data_state::get_by(){ return by;}
float data_state::get_bdegree(){ return bdegree;}
vector<int>& data_state::get_baction(){ return baction;}

vector<int>& data_state::get_list(){ return action_list;}
vector<int>& data_state::get_color(){ return cup_color;}
int data_state::get_cup(){ return cup;}
bool data_state::get_ns(){ return ns;}
int data_state::get_team(){ return team;}
int data_state::now_status(){ return status;}

void data_state::unityAction(){
	//ROS_INFO("SIZE %d", (int)action_list.size());
	for(int i = 0 ; i <= action_list.size() ; i++){
		if(saction_list[i] == 1 || baction_list[i] == 1)
			action_list[i] = 1;
		else{
			if(saction_list[i] == 2 || baction_list[i] == 2)
				action_list[i] = 2;
			else{
				if(saction_list[i] == 3 || baction_list[i] == 3)
					action_list[i] = 3;					
			}
		} 
	}
}

void data_state::tf_cup(){
	int radiu = 0;
	cup = 65535;
	// ROS_INFO("TF");
	for(int i = 0 ; i < 24 ; i++){
		for(int j = 0 ; j <= camera_cup_pos.size() ; j += 2){
			// ROS_INFO("TF_C");
			radiu = (camera_cup_pos[j] - bc[i].pos.first)*(camera_cup_pos[j] - bc[i].pos.first) + (camera_cup_pos[j+1] - bc[i].pos.second)*(camera_cup_pos[j+1] - bc[i].pos.second);
			if(radiu > 30*30){
				bc[i].state = false;
				break;
			}
			else{
				if(bc[i].color != camera_cup_color[j/2]){
					bc[i].state = false;
					break;
				}	
				else
					bc[i].state = true;
			}		
		}
		if(bc[i].state == false){
			// ROS_INFO("TF_F");
			cup = cup ^ (1 << i);
		}

	}
}

void data_state::unityCup(){
	//cup = 65535;
	tf_cup();
	cup = cup & (scup & bcup);
	//ROS_INFO("UNITY");
}

void data_state::datacallback(const main2021::Data::ConstPtr& msg){

	//small chicken
	sx = 0;//msg->small_chicken_pos[0];
	sy = 0;//msg->small_chicken_pos[1];
	sdegree = 0;//msg->small_chicken_pos[2];
	saction.assign(msg->small_action.begin(), msg->small_action.end());
	saction_list.assign(msg->small_action_list.begin(), msg->small_action_list.end());
	scup = 65535;//msg->small_cup;
	//big chicken
	bx = msg->big_chicken_pos[0];
	by = msg->big_chicken_pos[1];
	bdegree = msg->big_chicken_pos[2];
	baction.assign(msg->big_action.begin(), msg->big_action.end());
	baction_list.assign(msg->big_action_list.begin(), msg->big_action_list.end());
	bcup = msg->big_cup;

	team = msg->team;
	status = msg->status;
	//ROS_INFO("DATA");
}
void data_state::spinonce(){
	ros::spinOnce();
}
