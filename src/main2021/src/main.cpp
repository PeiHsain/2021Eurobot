#include <ros/ros.h>
#include "main2021/Data.h"
#include "main2021/dataToBig.h"
//#include "main2021/dataToSmall.h"
// #include "main2021/mission_srv.h"
// #include "main2021/missiontomain.h"
// #include "main2021/maintomission.h"
#include "main2021/plannerState.h"

#include "../include/main2021/state.h"
#include "../include/main2021/friend_state.h"
#include "../include/main2021/goap_state.h"
#include "../include/main2021/position_state.h"
#include "../include/main2021/mission_state.h"

#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

#define BEBLOCK 10*10 //mm
enum Status {STRATEGY = 0, RESET, UPDATE_DATA, GET_AGENT2, READY, RUN};
enum Mode {EMERGENCY = 0, NORMAL};
extern enum STATE state;// {FALSE = 0, SUCCESS, DOING, emerg};

float fx = 0;
float fy = 0;
float fdegree = 0;
vector<int> faction(2, 0);
vector<int> action_list(15, 0);
vector<int> cup_color(5, 0);
int cup = 65535;
bool ns = 0;
bool team = 0;
int status = 0;
float during_time = 0;

bool firstrun = false;
bool emerg_first = true;
int emerg_Time = 0;
// int getTime = 10;
//int planer_state = 0, mission_state = 0;
int cont = 0;
int mode = 1;
//int goap_action = 0;
//int odd_action = 0;

//int change = 0;

void datacallback(const main2021::dataToBig::ConstPtr& msg){
    fx = msg->small_x;
    fy = msg->small_y;
    fdegree = msg->small_degree;
    faction.assign(msg->small_action.begin(), msg->small_action.end());
    action_list.assign(msg->action_list.begin(), msg->action_list.end());
    cup_color.assign(msg->cup_color.begin(), msg->cup_color.end());
    cup = msg->cup;
    ns = msg->ns;
    team = msg->team;
    during_time = msg->time;
    status = msg->status;
    //ROS_INFO("GETDATA"); 
}

/*void datacallback(const main2021::dataToSmall::ConstPtr& msg){
    msg.big_x;
    msg.big_y;
    msg.big_degree;
    msg.small_action;
    msg.action_list;
    msg.cup_color;
    msg.cup;
    msg.ns;
    msg.team;
    msg.time;
}*/
bool beBlock(float x, float y, Position p){
    int deltax = 0, deltay = 0;
    deltax = (x - p.get_e1_x())*(x - p.get_e1_x());
    deltay = (y - p.get_e1_y())*(y - p.get_e1_y());
    if(deltax + deltay <= BEBLOCK)
        return true;
    deltax = (x - p.get_e2_x())*(x - p.get_e2_x());
    deltay = (y - p.get_e2_y())*(y - p.get_e2_y());
    if(deltax + deltay <= BEBLOCK)
        return true;

    return false;    
}

int mode_update(float x, float y, Position pos){
    if(beBlock(x, y, pos) == true){
        return EMERGENCY;
    }
    else{
        emerg_first = true;
        return NORMAL;
    }
}


int main(int argc, char** argv)
{
    //ROS initial
    ros::init(argc, argv, "main_node");
    ros::NodeHandle nh;

    ros::Publisher pub_data = nh.advertise<main2021::Data>("giveToData", 1000);
    ros::Subscriber sub_data = nh.subscribe<main2021::dataToBig>("DataToBig", 1000, datacallback);
    //ros::Subscriber sub_data = nh.subscribe<main2021::dataToSmall>("DataToSmall", 1000, datacallback);
    
    // main2021::mission_srv m_srv;
    main2021::Data give_data;
    main2021::dataToBig data_return;

    //service initial value
    // m_srv.request.action = {0};
    // m_srv.request.action_pos = {0};
    // m_srv.request.cup = {0};
    // m_srv.request.hand = {0};

    give_data.big_chicken_pos = {800, 200, 0};
    give_data.small_chicken_pos = {0, 0, 0};
    give_data.big_action = {0, 0};
    give_data.small_action = {0, 0};
    give_data.big_action_list = {0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0};
    give_data.small_action_list = {0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0};
    give_data.big_cup = 65535;
    give_data.small_cup = 65535;
    give_data.team = 0;
    give_data.status = 0;

    //定義value
    State states(800., 200., 0., 0.);
    Friend friends;
    Position position;
    goap_data goap;
    mission_data mission;

    //goap action 3 = go home
    while(ros::ok()){ //&& goap.getaction() != 3
        //status update
        states.set_status(status);
        ROS_INFO("STATUS:%d", states.get_status());
        //mode update (RUN) --> EMERGENCY or NORMAL
        //what time is it
        states.set_time(during_time);
        ROS_INFO("TIME:%f", states.get_time());
        //actions DONE or UNDO list --> 0 UNDO, 1 DONE
        states.set_list(&action_list);
        ROS_INFO("LIST[%d]", goap.getaction());
        //cup_state update
        //states.updatecup(cup);
        ROS_INFO("CUP:%d", states.get_cup());
        for (int i = 0; i < 12; i++)
        {
            ROS_INFO("HAND[%d]:%d", (i+1), states.get_hand()[i]);
        }
        //To Do********
        // if(cont != 1)
        
        states.setpos(position.get_px(), position.get_py(), position.get_pz(), position.getdegree());
        ROS_INFO("----------------");

        //更新Data//拿自己的位置,角度
        switch(states.get_status()){
            case STRATEGY://戰術選擇
                //To Do********
                give_data.status = states.get_status();
                //cont++;
                //change++;
                ROS_INFO("STRATEGY");
                break;
            case RESET://機構,底盤reset
                //To Do********
                give_data.status = states.get_status();
                //cont++;
                //change++;
                states.set_team(team);
                
                //call localization to get my position   
                ROS_INFO("RESET");
                break;
            case UPDATE_DATA://更新Data
                //cont++;
                //change++;
                give_data.status = states.get_status();
                give_data.big_chicken_pos[0] = states.getx();
                give_data.big_chicken_pos[1] = states.gety();
                give_data.big_chicken_pos[2] = states.getth();
                give_data.big_action = {0};
                give_data.big_action_list = {0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0};
                give_data.big_cup = 65535;
                /*give_data.small_chicken_pos.push_back(states.getx());
                give_data.small_chicken_pos.push_back(states.gety());
                give_data.small_chicken_pos.push_back(states.getdegree());
                give_data.small_action = {0};
                give_data.small_action_list = {0};
                give_data.small_cup = 65535;*/
                ROS_INFO("UPDATE_DATA");
                break;
            case GET_AGENT2://拿另一機的資料
                //cont++;
                //change++;
                give_data.status = states.get_status();

                // states.set_ns(ns);
                // states.set_color(&cup_color);

                friends.setf_x(fx);
                friends.setf_y(fy);
                friends.setf_z(0);
                friends.setf_degree(fdegree);
                friends.setf_action(&faction);
                ROS_INFO("GET_AGENT2");
                break;
            case READY://等拔插銷 from ST1
                give_data.status = states.get_status();
                //cont++;
                //change++;
                ROS_INFO("READY");
                break;
            case RUN:
                give_data.status = states.get_status();
                //cont++;
                ROS_INFO("RUN");
                ROS_INFO("x: %f", states.getx());
                ROS_INFO("y: %f", states.gety());
                //拿Data,self and friend的位置,角度
                //To Do********
                // states.setpos(float x, float y, float z, float degree_x, float degree_y, float degree_z, float degree_w);
                // states.set_cup(int c);
                // states.set_ns(bool n);
                // states.set_team(bool tm);
                // states.set_time(float t);

                friends.setf_x(fx);
                friends.setf_y(fy);
                friends.setf_z(0);
                friends.setf_degree(fdegree);
                friends.setf_action(&faction);

                mode = mode_update(states.getx(), states.gety(), position);
                switch(mode){
                    case EMERGENCY:
                    //To Do*******
                        ROS_INFO("EMERGENCY");
                        if(emerg_first == true){
                            emerg_Time = states.get_time() + 5;//最長的急停忍受時間
                            emerg_first = false;
                            ROS_INFO("EMERG FIRST");
                        }
                        states.set_p_state(emerg); //planer(emergency);
                        states.set_m_state(emerg); //mission(emergency);
                        ROS_INFO("EMERG P");
                        position.give_plan(states.getx(), states.gety(), states.getth());
                        ROS_INFO("EMERG M");
                        mission.give_mission(states, goap);
                        ROS_INFO("EMERG DURING");
                        if(states.get_time() >= emerg_Time){
                            ROS_INFO("EMERG GOAP");
                            goap.give_goap(states, friends, position);
                            ROS_INFO("EMERG PLAN");
                            position.give_plan(goap.get_action_x(), goap.get_action_y(), goap.get_action_th()); 
                            // states.setpos(goap.get_action_x(), goap.get_action_y(), 0, goap.get_action_th());
                            // cont = 1;
                        }
                        break;          

                    case NORMAL:
                        ROS_INFO("NORMAL");
                        if(states.get_p_state() == emerg && states.get_m_state() == emerg){
                            ROS_INFO("GOAP_EMERG");
                            states.set_emerg(false);
                            goap.give_goap(states, friends, position);
                            //ROS_INFO("E_DONE");
                        }
                        if(states.get_p_state() == FALSE){
                            ROS_INFO("GOAP_PFALSE");
                            goap.give_goap(states, friends, position);
                            //ROS_INFO("P_DONE");
                        }
                        else{
                            if(states.get_m_state() == SUCCESS){
                                switch (goap.getaction())
                                {
                                case 12: //get cup
                                    if(((1 << (goap.get_action_cup()-1)) & cup) != 0)
                                        break;
                                    else
                                        goap.give_goap(states, friends, position);
                                
                                case 13: //get cup1 and cup2
                                    if((3 & cup) != 0)
                                        break;
                                    else
                                        goap.give_goap(states, friends, position);
                        
                                case 14: //get cup3 and cup4
                                    if((12 & cup) != 0)
                                        break;
                                    else
                                        goap.give_goap(states, friends, position);
                                default:
                                    goap.give_goap(states, friends, position);
                                }
                                ROS_INFO("GOAP_M");
                                
                            }
                            else if(states.get_m_state() == FALSE){
                                ROS_INFO("GOAP_M");
                                goap.give_goap(states, friends, position);
                            }
                            ROS_INFO("SAME ACTION:%d", goap.sameActionOrNot());
                            //ROS_INFO("M_DONE");
                        }

                        
                        ROS_INFO("P_STATE:%d", states.get_p_state());
                        if(goap.sameActionOrNot() == false || states.get_p_state() != SUCCESS){
                            ROS_INFO("PLANNER");
                            position.give_plan(goap.get_action_x(), goap.get_action_y(), goap.get_action_th());
                        }
                        //To Do*******
                        mission.give_mission(states, goap);

                        //更新Data
                        //planner state update
                        states.set_p_state(position.get_p_state());
                        states.set_m_state(mission.getstate(states, goap));
                        ROS_INFO("M:%d", states.get_m_state());
                        ROS_INFO("NOW:%d", goap.getaction());
                        if(states.get_m_state() == SUCCESS){
                            action_list[goap.getaction()] = 1;
                            //ROS_INFO("1:%d", action_list[goap.getaction()]);
                            states.set_hand(&mission.get_hand());
                            states.set_cup(goap.get_action_cup(), goap.getaction());                           
                        }
                        else if(states.get_m_state() == DOING)
                            action_list[goap.getaction()] = 2;
                        else if(states.get_m_state() == FALSE)
                            action_list[goap.getaction()] = 3;
                        give_data.big_chicken_pos[0] = states.getx();
                        give_data.big_chicken_pos[1] = states.gety();
                        give_data.big_chicken_pos[2] = states.getth();
                        give_data.big_action[0] = goap.getaction();
                        give_data.big_action[1] = goap.get_action_x();
                        give_data.big_action[2] = goap.get_action_y();
                        give_data.big_action[3] = goap.get_action_th();
                        give_data.big_action[4] = goap.get_action_cup();
                        give_data.big_action[5] = goap.get_action_hand();
                        give_data.big_action_list.assign(action_list.begin(), action_list.end());
                        give_data.big_cup = states.get_cup();
                        ROS_INFO("UPDATE_DONE: %d",give_data.big_action_list[2]);
                        break;
                }                   
        }
        //ROS_INFO("GIVE DATA");
        pub_data.publish(give_data);
        ros::spinOnce();
    }
    return 0;
}
