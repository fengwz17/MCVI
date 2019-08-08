#ifndef __BELIEF_H
#define __BELIEF_H

#include "State.h"
#include "Utils.h" 
#include "Gauss.h"


#include <vector>
#include <map>
#include <list>

// 令置信度分布为满足给定均值的高斯分布
struct BeliefNode
{
    double Ex1, Ey1;
    double Ex2, Ey2;
};

class Belief
{
    public:

        // initialize the beliefNode associated with the belief
        static void initBelief(BeliefNode* beliefNode) 
        {
            beliefNode->Ex1 = 0;
            beliefNode->Ey1 = 0;
            //beliefNode->Ex2 = random(100);
            //beliefNode->Ey2 = random(100) - 50;
            beliefNode->Ex2 = 50;
            beliefNode->Ey2 = 0;
        }
                    

        // sample from this belief
        // return a state sampled from this belief
        static State sample(BeliefNode beliefNode);
        
        // 确定性状态转移
        static State MDPsample(BeliefNode beliefNode);

        // compute the next belief given an action an an obs
        BeliefNode nextBelief(BeliefNode currBelief, double action);

};


#endif