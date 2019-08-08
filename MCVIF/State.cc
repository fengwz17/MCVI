#include "State.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

void State::initstate()
{
    // 定义初始两个uav的位置
    // this->x1 = GaussSample(0, 1);
    // this->y1 = GaussSample(0, 1);
    this->x1 = 0;
    this->y1 = 0;

    //this->x2 = random(60);
    //this->y2 = random(60) - 30;
    this->x2 = 50;
    this->y2 = 0;
    
    // 初始角度
    this->theta1 = 0;
    this->theta2 = PI;

    // 水平速度为常数
    this->u = 1;
}

State State::trans(double a)
{
    // intruder的动作
    double aIntruder;
    aIntruder = act[int(random(3))];

    State nextState;
    nextState.initstate();
    nextState.theta1 = this->theta1 + a;
    nextState.theta2 = this->theta2 + aIntruder;
    nextState.u = this->u;

    // cout << "u: " << nextState.u << endl;
    nextState.x1 = posTrans(this->u, nextState.theta1, this->x1, 0);
    nextState.y1 = posTrans(this->u, nextState.theta1, this->y1, 1);

    nextState.x2 = posTrans(this->u, nextState.theta2, this->x2, 0);
    nextState.y2 = posTrans(this->u, nextState.theta2, this->y2, 1);
    // cout << "nextState: " << nextState.x1 << " " << nextState.y1 << endl;
        
    return nextState;
}