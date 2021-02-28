#ifndef DATA_STATE_H_
#define DATA_STATE_H_

#include <ros/ros.h>
#include "main2021/Data.h"
#include "main2021/cup_camera.h"

#include "../include/main2021/data_state.h"

#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

typedef struct cup_p
{
	int num;
	pair<float, float> pos;
	int color;
	bool state;
}CUP;

class data_state{
public:
	data_state();
	void initial_cup_pos();
	void callCamera(bool);


	float get_sx();
	float get_sy();
	float get_sdegree();
	vector<int>& get_saction();

	float get_bx();
	float get_by();
	float get_bdegree();
	vector<int>& get_baction();

	vector<int>& get_list();
	vector<int>& get_color();
	int get_cup();
	bool get_ns();
	int get_team();
	int now_status();

	void unityAction();

	void tf_cup();

	void unityCup();

	void datacallback(const main2021::Data::ConstPtr&);
	void spinonce();

private:
	ros::NodeHandle h;
	ros::Subscriber sub_data;
	ros::ServiceClient client_camera;
	main2021::cup_camera c_srv;

	float sx;
	float sy;
	float sdegree;
	vector<int> saction;
	vector<int> saction_list;
	int scup;
	
	float bx;
	float by;
	float bdegree;
	vector<int> baction;
	vector<int> baction_list;
	int bcup;
	
	vector<int> action_list;
	vector<int> cup_color;
	vector<float> cup_pos;
	int cup;
	bool ns;
	bool team;
	int status;
	CUP bc[24];
};

#endif
