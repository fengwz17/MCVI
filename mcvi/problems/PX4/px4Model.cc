#include "px4Model.h"
#include "Include.h"
#include "ParticlesBelief.h"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <deque>

#include <iostream>

using namespace std;

// int indoor(double pos) {
//     int i=0;
//     for(i=0;i<NumDoors;++i)
//         if(fabs(DoorPositions[i]-pos)<=DoorRadius)
//             return i;
//     return -1;
// }


// 在状态s下得到观测值
// 这里将对方的坐标值加上一个高斯扰动来表示传感器误差
// return 0 表示no-detection
// return 1,2 分别表示左右
int observe(position pos)
{
    // 这时两个uav的坐标：s->x1, s->y1; x2, y2
    // own uav的朝向s->theta1
    // 必较两个uav的相对位置，把坐标系intruder的坐标变换一下
    // 变换之后与零点比较
    double x22, y22;
    x22 = (x2 - x1) * sin(theta1) - (y2 - y1) * cos(theta1);
    y22 = (x2 - x1) * cos(theta1) + (y2 - y1) * sin(theta1);

    // 进入探测范围
    if(distance(0, x22, 0, y22) < D)
    {
        if(y22 >= 0 && x22 >= 0)
            return 1;
        else if(y22 >= 0 && x22 < 0)
            return 2;
        else
            return 0;
    }
}

// /* add noise to the real value,
//  * eg. a true value has 0.05 chance become false
//  */
// //TODO check this, this seems incorrect
// //this is correct
// inline bool noisy(bool v)
// {
//     return (v ^ (randf()<Noise));
// }

// #define NOISY(v) ((v) ^ (randStream->getf()<Noise))

px4Model::px4Model(int numParticles):
        Model(NumStateVars, NumObsVars, NumActs, NumMacroActs, NumInitPolicies, Discount),
        numParticles(numParticles)
{
}

bool px4Model::allowableAct(Belief const& belief, Action const& action)
{
    if (action.type == Macro) return false;

    return true;
}

double px4Model::sample(State const& currState, Action const& action, State* nextState, Obs* obs, RandStream* randStream )
{
/*     
    double reward = MovementCost;
    if (currState[0] < 0){ // terminal state
        obs->obs[0] = TermObs;
        (*nextState) = currState;
        if (action.getActNumUser()==ActEnter)
            return WrongPenalty;
        else
            return 0;
    };

    double pos = currState[1];


    if (action.getActNumUser()==ActEnter) {
        if(indoor(pos)==0) {
            (*nextState)[0] = TermState;
            (*nextState)[1] = currState[1];
            obs->obs[0] = TermObs;
            reward = EnterReward;
        } else {
            (*nextState) = currState;
            obs->obs[0] = ObsNothing;
            reward = WrongPenalty;
        }
    } else {
        assert(action.getActNumUser()==ActLeft|| action.getActNumUser()==ActRight);
        double moveDir;
        double randReal = randStream->getf();
        double nxtPos;

        if(action.getActNumUser()==ActLeft) moveDir = -1;
        else moveDir = 1;
        moveDir += (randReal - 0.5)*Noise;

        nxtPos = pos + moveDir;

        obs->obs[0]=ObsNothing;
        if(nxtPos<-CorridorLength) {
            // crash left
            nxtPos = -CorridorLength;
            obs->obs[0] = ObsCrash;
            reward = WrongPenalty;
        } else if(nxtPos>CorridorLength) {
            // crash right
            nxtPos = CorridorLength;
            obs->obs[0] = ObsCrash;
            reward = WrongPenalty;
        } else if( (nxtPos>=CorridorLength-CorridorEndLength) ) {
            // see corridor right end
            obs->obs[0] = ObsRightEnd;
        } else if( (nxtPos<=-CorridorLength+CorridorEndLength) ) {
            // see corridor left end
            obs->obs[0] = ObsLeftEnd;
        } else if( NOISY(indoor(nxtPos)>=0)) {
            obs->obs[0] = ObsDoor;
        }

        (*nextState)[1] = nxtPos;
        (*nextState)[0] = 0;
    }
*/
    double reward = turn;
    if (currState[0] < 0){ // terminal state
        obs->obs[0] = TermObs;
        (*nextState) = currState;
        if (action.getActNumUser()==ActEnter)
            return WrongPenalty;
        else
            return 0;
    };

    double pos = currState[1];


    if (action.getActNumUser()==ActEnter) {
        if(indoor(pos)==0) {
            (*nextState)[0] = TermState;
            (*nextState)[1] = currState[1];
            obs->obs[0] = TermObs;
            reward = EnterReward;
        } else {
            (*nextState) = currState;
            obs->obs[0] = ObsNothing;
            reward = WrongPenalty;
        }
    } else {
        assert(action.getActNumUser()==ActLeft|| action.getActNumUser()==ActRight);
        double moveDir;
        double randReal = randStream->getf();
        double nxtPos;

        if(action.getActNumUser()==ActLeft) moveDir = -1;
        else moveDir = 1;
        moveDir += (randReal - 0.5)*Noise;

        nxtPos = pos + moveDir;

        obs->obs[0]=ObsNothing;
        if(nxtPos<-CorridorLength) {
            // crash left
            nxtPos = -CorridorLength;
            obs->obs[0] = ObsCrash;
            reward = WrongPenalty;
        } else if(nxtPos>CorridorLength) {
            // crash right
            nxtPos = CorridorLength;
            obs->obs[0] = ObsCrash;
            reward = WrongPenalty;
        } else if( (nxtPos>=CorridorLength-CorridorEndLength) ) {
            // see corridor right end
            obs->obs[0] = ObsRightEnd;
        } else if( (nxtPos<=-CorridorLength+CorridorEndLength) ) {
            // see corridor left end
            obs->obs[0] = ObsLeftEnd;
        } else if( NOISY(indoor(nxtPos)>=0)) {
            obs->obs[0] = ObsDoor;
        }

        (*nextState)[1] = nxtPos;
        (*nextState)[0] = 0;
    }
    return reward;
}

double px4Model::sample(State const& currState, Action const& macroAction, long controllerState, State* nextState, long* nextControllerState, Obs* obs, RandStream* randStream)
{
    // should never reach here
    assert(false);
    return 0;
}


double px4Model::initPolicy(State const& currState, Action const& initAction, long controllerState, State* nextState, long* nextControllerState, Obs* dummy, RandStream* randStream)
{
    Obs obs(vector<long>(getNumObsVar(), 0));
    if (currState[0] < 0){
        (*nextState) = currState;
        return 0;
    };

    return sample(currState, Action(Act,Goahead), nextState, &obs, randStream);
}

State px4Model::sampleInitState() const
{
    double p = randf();
    p = p*CorridorLength*2 - CorridorLength ;
    State st(getNumStateVar(), 0);
    st[1] = p;
    return st;
}

/* Should be a tight upper bound, right? */
double px4Model::upperBound(State const& state)
{
    double minSteps = fabs(state[1] - DoorPositions[0]);
    double reward = pow(discount, minSteps) * EnterReward;
    return reward;
}

double px4Model::getObsProb(Action const& action, State const& nextState, Obs const& obs)
{
/* 
    double nxtPos = nextState[1];
    int ndoor = indoor(nxtPos);

    if(obs.obs[0]==TermObs) {
        if(nextState[0]==TermState)
            return 1.0;
        else
            return 0.0;
    }else if(obs.obs[0]==ObsCrash) {
        if(nxtPos<=0 || nxtPos>=CorridorLength)
            return 1.0;
        else
            return 0.0;
    }else if(obs.obs[0]==ObsDoor) {
        if(ndoor>=0)
            return 1-Noise;
        else
            return 0;
    }else if(obs.obs[0]==ObsLeftEnd) {
        if(nxtPos<=-CorridorLength+CorridorEndLength)
            return 1;
        else
            return 0;
    }else if(obs.obs[0]==ObsRightEnd) {
        if(nxtPos>=CorridorLength-CorridorEndLength)
            return 1;
        else
            return 0;
    } else {
        assert(obs.obs[0]==ObsNothing);
        if(nxtPos<=-CorridorLength || nxtPos>=CorridorLength)
            return 0.0;
        else if(ndoor>=0)
            return Noise;
        else if((nxtPos<=-CorridorLength+CorridorEndLength) ||
                (nxtPos>=CorridorLength-CorridorEndLength))
            return 0;
        else
            return 1.0;
    }
*/
    double nxtPos = nextState[1];
    int ndoor = indoor(nxtPos);

    if(obs.obs[0]==TermObs) {
        if(nextState[0]==TermState)
            return 1.0;
        else
            return 0.0;
    }else if(obs.obs[0]==ObsCrash) {
        if(nxtPos<=0 || nxtPos>=CorridorLength)
            return 1.0;
        else
            return 0.0;
    }else if(obs.obs[0]==ObsDoor) {
        if(ndoor>=0)
            return 1-Noise;
        else
            return 0;
    }else if(obs.obs[0]==ObsLeftEnd) {
        if(nxtPos<=-CorridorLength+CorridorEndLength)
            return 1;
        else
            return 0;
    }else if(obs.obs[0]==ObsRightEnd) {
        if(nxtPos>=CorridorLength-CorridorEndLength)
            return 1;
        else
            return 0;
    } else {
        assert(obs.obs[0]==ObsNothing);
        if(nxtPos<=-CorridorLength || nxtPos>=CorridorLength)
            return 0.0;
        else if(ndoor>=0)
            return Noise;
        else if((nxtPos<=-CorridorLength+CorridorEndLength) ||
                (nxtPos>=CorridorLength-CorridorEndLength))
            return 0;
        else
            return 1.0;
    }
}

void px4Model::displayState(State state, long type)
{
    cout << state[1] << endl;
}
