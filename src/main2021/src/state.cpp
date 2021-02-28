#include <ros/ros.h>

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "../include/main2021/state.h"
#include "../include/main2021/goap_state.h"


using namespace std;

State::State(float x, float y, float z, float th){
    my_x = x;
    my_y = y;
    my_z = z;
    degree = th;
    cup_color = {0};
    hand = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    cup = 65535;
    ns = 0;
    team = 0;
    emerg = false;
    time = 0;
    strategy = 0;
    status = 0;
    planer_state = SUCCESS;
    mission_state = SUCCESS;
}


float State::getx(){ return my_x;}
float State::gety(){ return my_y;}
float State::getz(){ return my_z;}
float State::getth(){ return degree;}

std::vector<int>& State::get_hand(){ return hand;}
std::vector<int>& State::get_list(){ return actionList;}
int State::get_cup(){ return cup;}
bool State::get_ns(){ return ns;}
bool State::get_team(){ return team;}
bool State::emergOrNot(){ return emerg;}
float State::get_time(){ return time;}
int State::get_strategy(){ return strategy;}    
int State::get_status(){ return status;}

int State::get_p_state(){ return planer_state;}
int State::get_m_state(){ return mission_state;}

void State::setpos(float x, float y, float z, float th){
    my_x = x;
    my_y = y;
    my_z = z;
    degree = th;       
}
void State::set_hand(std::vector<int>* h){
    hand.assign(h->begin(), h->end());
}
void State::set_list(std::vector<int>* a){
    actionList.assign(a->begin(), a->end());
}
void State::set_cup(int c, int m){
    if(m == 12){
        c = 1 << (c-1);
        ROS_INFO("state c: %d", c);
        ROS_INFO("state cup 1: %d", cup);
        if((c & cup) != 0)
            cup = (c ^ cup);
        ROS_INFO("state cup 2: %d", cup);
    }
    else if(m == 13){ //get cup1 and cup2
        if((3 & cup) != 0)
            cup = (3 ^ cup);
    } 
    else if(m == 14){ //get cup3 and cup4
        if((12 & cup) != 0)
            cup = (12 ^ cup);
    }
}
void State::updatecup(int c){ cup = c;}
void State::set_ns(bool n){ ns = n;}
void State::set_team(bool tm){ team = tm;}
void State::set_time(float t){ time = t;}
void State::set_status(int s){ status = s;}
void State::set_p_state(int state){ planer_state = state;}
void State::set_m_state(int state){ 
    if(state == 3)
        emerg = true;
    mission_state = state;
}
void State::set_emerg(bool e){ emerg = e;}
