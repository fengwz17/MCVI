#ifndef __GAUSS_H
#define __GAUSS_H

#include "Utils.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

// random sample
inline double GaussSample(double E, double var)
{
    static double v1, v2, S;
    static int phase = 0;
    double X;

    if(phase == 0)
    {
        do {
            double u1 = (double)rand() / RAND_MAX;
            double u2 = (double)rand() / RAND_MAX;

            v1 = 2 * u1 - 1;
            v2 = 2 * u2 - 1;
            S = v1 * v1 + v2 * v2;
        } while(S >= 1 || S == 0);

        X = v1 * sqrt(-2 * log(S) / S);
    }
    else
        X = v2 * sqrt(-2 * log(S) / S);
    
    phase = 1 - phase;
    X = X * sqrt(var) + E;
    return X;
}

inline double evaluate(double x, double mean, double var)
{
    double factor = sqrt(2 * PI * var);
    return exp(-(x - mean) * (x - mean) / (2 * var)) / factor;
}

inline double random(int x)
{
    return ((double)(rand() % x));
}

#endif

/*
    // double E, var;
    // cin >> E >> var;
    // int k = 0;
    // while(k <= 30)
    // {
    //     cout << evaluate(sample(E,var), E, var) << endl;

    //     k++;
    // }
    // srand((int)time(0));
    // for(int k = 0; k < 100; k++)
    // {
    //     //cout << random(0,100) << endl;
        
    //     cout << random(100) << endl;
    //     cout << random(100) - 50 << endl;
    // }
    // system("pause");
*/