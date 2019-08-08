#ifndef __UTILS_H
#define __UTILS_H

//#include "Gauss.h"
#include <cmath>
#include <cstdlib>
#include <vector>

#include <limits>
#include <time.h>
#include <iostream>

using namespace std;

const double length = 100;
const double width = 100;

const double epsilon = 1e-5;

const long Never = -1;
#define Inf (std::numeric_limits<double>::infinity())
#define NegInf (-(std::numeric_limits<double>::max()-1))

#define PI 3.14159265358979323846
#define ND 50

const double NMAC = 10;
const double Wm = PI / 4;

const int L = 100;
const double gamma = 0.2;

const int obs[3] = {0, 1, 2};
const double act[3] = {0, -Wm, Wm};

inline double power(double x, long i)
{
    if (i == 0)
        return 1;
    double temp = power(x, i/2);
    if (i % 2 == 0)
        return temp * temp;
    else
        return temp * temp * x;
}

inline bool equal(double x, double y)
{
    return (fabs(y - x) <= epsilon);
}

inline double distance(double x1, double x2, double y1, double y2)
{
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

inline double posTrans(double u, double thelta, double xy, int flag)
{
    double nextX = 0;
    double nextY = 0;

    // x轴变化
    if (flag == 0)
    {
        // cout<<"u*cos"<< u * cos(thelta) <<endl;
        nextX = xy + u * cos(thelta);
        return nextX;
    }

    // y轴变化
    else
    {
        nextY = xy + u * sin(thelta);
        return nextY;
    }
}

inline double reward(double a, double x1, double x2, double y1, double y2)
{
    // if(x1 < 0 || x1 > 100 || y1 < -50 || y1 > 50)
    //     return -100;
    // else
    // {
        if(distance(x1, x2, y1, y2) <= NMAC)
            return -10000;
        else
        {
            if(a == 0)
                return 0;
            else
                return -0.1;
        }
    //}
}


#endif