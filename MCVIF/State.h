#ifndef __STATE_H
#define __STATE_H

#include "Gauss.h"
#include "Utils.h"

struct State
{
        double x1, y1, theta1;
        double x2, y2, theta2;
        double u = 1;

        void initstate();
        State trans(double a);
     
};

#endif