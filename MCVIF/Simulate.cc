
#include "Simulate.h" 

// 在状态s下得到观测值
// 这里将对方的坐标值加上一个高斯扰动来表示传感器误差
// return 0 表示no-detection
// return 1,2 分别表示左右
int observe(State* s, bool flag)
{
    double x2;
    double y2;

    // flag == 1表示对intruder坐标做高斯干扰
    if(flag == 1)
    {
        x2 = GaussSample(s->x2, 1);
        y2 = GaussSample(s->y2, 1);
    }

    // flag == 0时表示确定性观测
    else
    {
        x2 = s->x2;
        y2 = s->y2;
    }
    
    // 这时两个uav的坐标：s->x1, s->y1; x2, y2
    // own uav的朝向s->theta1
    // 必较两个uav的相对位置，把坐标系intruder的坐标变换一下
    // 变换之后与零点比较
    double theta = s->theta1;
    double x22, y22;
    x22 = (x2 - s->x1) * sin(theta) - (y2 - s->y1) * cos(theta);
    y22 = (x2 - s->x1) * cos(theta) + (y2 - s->y1) * sin(theta);

    // 进入探测范围
    if(distance(0, x22, 0, y22) < ND)
    {
        if(y22 >= 0)
        {
            if(x22 >= 0)
                return 1;
            else
                return 2;
        }
        else
            return 0;
    }
    else 
        return 0;
}

double simulate(Graph G, Graph::Node startNode, State startState, int L, double gamma, bool flag)
{
    double currReward;
    double V = 0;
    double rewardS = 0;

    State currState = startState;
    State nextState;
    nextState.initstate();

    double action;
    
    Graph::Node currNode = startNode;
    Graph::Node nextNode;

    // Graph::Node tmpNode;

    double obs;

    //double dis;
    //dis = distance(currState.x1, 0, currState.y1, 0);

    for(int i = 0; i < L; ++i)
    {

        rewardS = reward(currNode.action, currState.x1, currState.x2, currState.y1, currState.y2);
        // V += power(gamma, i) * rewardS;
        for(int j = 0; j < i; j++)
        {
            rewardS *= gamma;
        }
        V += rewardS;
        // cout<<'V'<<V;
        // 从当前状态中得到观测
        obs = observe(&currState, flag);
        
        Graph::Edge nextEdge;
        nextEdge = currNode.nextEdge;

        // nextEdge.obsEdge[obs]表示对应于obs的节点编号
        nextNode = G.NodeSet[nextEdge.obsEdge[obs]];

        // 得到对应于obs的节点保存的action
        // 在当前状态下执行action
        action = nextNode.action;

        //dis = distance(currState.x1, 0, currState.y1, 0);
        
        // 状态转换
        nextState = currState.trans(action);
        // cout<<"action" << action<<endl;
        // cout << "currState: " << "x1: " << currState.x1 << "y1: " << currState.y1 << "x2: " << currState.x2 << "y2: " << currState.y2 << endl;
        // cout << "nextState: " << "x1: " << nextState.x1 << "y1: " << nextState.y1 << "x2: " << nextState.x2 << "y2: " << nextState.y2 << endl;
        
        // currNode = currNode->nextNode
        // tmpNode = currNode;
        currNode = nextNode;
        // nextNode = tmpNode;

        // currState = currState->nextState
        State tmp;
        tmp = currState;
        currState = nextState;
        nextState = tmp;
    }
    
    return V;
}