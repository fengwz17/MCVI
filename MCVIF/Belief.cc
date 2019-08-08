#include <vector>
#include <map>
#include <list>
#include "Belief.h"
#include "State.h"


// 从beliefNode中采样
// beliefNode记录了Ex1，Ey1，Ex2，Ey2，表示两个坐标分布的均值
// 从beliefNode中采样即得到满足给定均值的高斯分布变量x1,y1,x2,y2
State Belief::sample(BeliefNode beliefNode)
{
    State state;
    state.initstate();
    // cout << "beliefNode: " << beliefNode.Ex1 << endl;
    // cout << "beliefNode: " << beliefNode.Ey1 << endl;
    // cout << "beliefNode: " << beliefNode.Ex2 << endl;
    // cout << "beliefNode: " << beliefNode.Ey2 << endl;

    state.x1 = GaussSample(beliefNode.Ex1, 1);
    state.y1 = GaussSample(beliefNode.Ey1, 1);
    state.x2 = GaussSample(beliefNode.Ex2, 1);
    state.y2 = GaussSample(beliefNode.Ey2, 1);
    // cout << "state: " << state.x1 << endl;
    // cout << "state: " << state.y1 << endl;
    // cout << "state: " << state.x2 << endl;
    // cout << "state: " << state.y2 << endl;


    return state;
}

State Belief::MDPsample(BeliefNode beliefNode)
{
    State state;
    state.initstate();
    state.x1 = beliefNode.Ex1;
    state.y1 = beliefNode.Ey1;
    state.x2 = beliefNode.Ex2;
    state.y2 = beliefNode.Ey2;
    return state;
}

// 生成belief树时，从当前belief生成下一个状态的belief
// 在当前belief下执行动作a，得到可能的下一个状态s
// 通过观测o得到状态s的置信度分布，即要利用预先给出的概率转移函数Z = p(o|s,a)
// 
// 在这里的问题中，s用两个坐标表示，坐标的分布为高斯分布，
// 每次状态转移改变坐标值，新的状态中坐标值依然满足高斯分布
// 于是新的belief用以新状态坐标为均值的高斯分布表示
BeliefNode Belief::nextBelief(BeliefNode currBelief, double action)
{
    State currState;
    currState = Belief::MDPsample(currBelief);
    State nextState;
    nextState = currState.trans(action);

    BeliefNode nextBeliefNode;
    nextBeliefNode.Ex1 = nextState.x1;
    nextBeliefNode.Ey1 = nextState.y1;
    nextBeliefNode.Ex2 = nextState.x2;
    nextBeliefNode.Ey2 = nextState.y2;

    return nextBeliefNode;
}

