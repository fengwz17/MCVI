#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <stack>
#include <vector>

#include "ValueIteration.h"
#include "Belief.h"
#include "Utils.h"

using namespace std;


void ValueIteration::MCBackup(Graph &G, BeliefNode beliefNode, int N)
{
    long nodeSize;
    nodeSize = G.NodeSet.size();

    double R[numActions] = {0};

    double Va[numActions] = {0}; // 执行动作a得到的最大reward

    // 执行动作a，得到观测o，以v为顶点的最大reward
    // 这个变量用来遍历求最优值
    double V[numActions][3][nodeSize]; 

    int bestaNum = 0; // 最好的action
    double bestVa = NegInf;

    // 记录边aov中顶点v的id
    long bestAovId[numActions][3];

    // double bestValue = NegInf;
    // long bestAction = 0;
    // take a in state si, 遍历a
    //cout << "aaabbb" << endl;
    for (long j = 0; j < numActions; j++)
    {
        //cout << "N=10" << endl;
        int obs[3] = {0};
        srand((unsigned)time(NULL)); 
        for (int i = 0; i < N; i++)
        {
            State si;
            si.initstate(); 
            
            cout << "si: " << " x1: " << si.x1 << " y1: " << si.y1 
                << " x2: " << si.x2 << " y2: " << si.y2 << endl;

            // 从b中采样得到一个状态si
            // 在这里就是以b中保存的值为均值，取满足给定均值、方差为1高斯分布的变量
            si = Belief::sample(beliefNode);

            cout << "beliefNode: " << " Ex1: " << beliefNode.Ex1 << " Ey1: " << beliefNode.Ey1 
                 << " Ex2: " << beliefNode.Ex2 << " Ey2: " << beliefNode.Ey2 << endl;

            cout << "si: " << " x1: " << si.x1 << " y1: " << si.y1 
                 << " x2: " << si.x2 << " y2: " << si.y2 << " theta1: " << si.theta1 << endl;

            State siNext;

            cout << "act[" << j << "] = " << act[j] << endl;
            siNext = si.trans(act[j]);

            cout << "siNext: " << " x1: " << siNext.x1 << " y1: " << siNext.y1 
                 << " x2: " << siNext.x2 << " y2: " << siNext.y2 << endl;
            
            obs[i] = observe(&siNext, 0);
            cout << " obs[" << i << "] = " << obs[i] << endl;
            
            double Rsia = reward(act[j], si.x1, si.x2, si.y1, si.y2);
            R[j] = R[j] + Rsia; 

            double Va = 0;

            // 当前采样得到o是obs[i]，遍历一个图G，记录以每个顶点为起始点的reward和
            // 找到最大的那个顶点，作为边obs[i]的端点

            //cout << "dd" << endl;
            // 遍历图G中的点
            for(auto iter = G.NodeSet.cbegin(); iter != G.NodeSet.cend(); iter++)
            {
                //cout << "iter->first: " << iter->first << endl;
                // Va是以iter->first这个编号的顶点为起始点，
                // 走L步得到的reward和
                Va = simulate(G, iter->second, siNext, L, gamma, 0);

                // 把N次采样的结果都累加起来
                V[j][obs[i]][iter->first] = V[j][obs[i]][iter->first] + Va;

            }
        }

        double Vao[numActions][3] = {0};
       
        double sumVao = 0;
        double bestV;
        double bestvId;

        /* 对于当前的action，需要得到一条边aov
         * a表示新加点，新加点的action为a
         * 循环o,对每一个o，连接G中的顶点v
         * 使以v为起始点做simulate的reward和最大
         * 这里表示v应该要返回一个编号
        */
        for(int i = 0; i < 3; ++i)
        {
            // bestV和bestvId表示对于固定a,o来说最好的下个动作
            bestV = NegInf;
            bestvId = 0;

            for(long v = 0; v < nodeSize; ++v)
            {
                cout << "v.action: " << G.NodeSet[v].action << endl;
                cout << "V[" << j << "][" << i << "][" << v << "]: "  << V[j][i][v] << endl; 
                if(V[j][i][v] >= bestV)
                {
                    bestV = V[j][i][v];
                    bestvId = v;
                }
            }
            
            Vao[j][i] = bestV;

            // 对于当前a，对每一个o，最好的v的编号
            bestAovId[j][i] = bestvId; 
            sumVao += Vao[j][i];
            cout << "observe: " << i << " bestvId: " << bestvId << endl;
            cout << "observe: " << i << " sumVao: " << sumVao << endl;
        }

        Va[j] = (R[j] + gamma * sumVao) / N;
        cout << "Va[" << j << "] = " << Va[j] << endl;
        cout << "bestVa = " << bestVa << endl; 
 
        // bestaNum是做MC-backup后得到的最好的action number
        if(Va[j] >= bestVa)
        {
            bestVa = Va[j];
            bestaNum = j;
        }
    
    }

    cout << "bestVa: " << bestVa << endl; 
    cout << "bestaNum: " << bestaNum << endl;
   
    // 得到besta为新加入图的点的action

    Graph::Node newNode;
    Graph::Edge newEdge;

    newNode.nodeNum = nodeSize;
    newNode.action = act[bestaNum];
   
    for(int i = 0; i < 3; ++i)
    {
        // 新加入节点引出3条边
        newEdge.obsEdge[i] = bestAovId[bestaNum][i];
    }

    newNode.nextEdge = newEdge;

    G.NodeSet[nodeSize] = newNode;  

    //G.allNodes.push_back(newNode);
    //cout<<G.NodeSet.size()<<" "<<G.allNodes.size()<<endl;
        // NodeSet.insert(action[bestAction]);

        //cout << "time: " << difftime(curr,start) << " Diff: " << currChange << "\n";
    //}    
    // return bestV;
}

// 计算lowerBound = V_G(b)
double ValueIteration::LowerBound(BeliefNode belief, Graph G)
{
    int L = 100;
    int NodeLength = G.NodeSet.size();
    double bestV = NegInf;
    double currV;
    
    for(int i = 0; i < NodeLength; ++i)
    {
        currV = simulate(G, G.NodeSet[i], Belief::sample(belief), L, gamma, 1);
        //cout<<"cuurV "<<currV<<endl;
        if(currV >= bestV)
        {
            bestV = currV;
        }
    }

    return bestV;
}

// 用确定性状态计算upperBound
// 转移中的状态是确定性状态
// 从状态中得到的观测是确定性的观测
double ValueIteration::upperBound(BeliefNode belief, Graph G)
{
    int L = 100;
    int NodeLength = G.NodeSet.size();
    double bestV = NegInf;
    double currV;
    
    for(int i = 0; i < NodeLength; ++i)
    {
        // 确定性状态转移
        currV = simulate(G, G.NodeSet[i], Belief::MDPsample(belief), L, gamma, 0);
        if(currV >= bestV)
        {
            bestV = currV;
        }
    }

    return bestV;
}

void ValueIteration::show()
{
    cout<<"dddd";
}

void ValueIteration::doIteration()
{
    double lowerBound;
    int epoch = 0;

    cout << epoch << endl;

    // belief初始化
    BeliefNode initBeliefNode;
    Belief::initBelief(&initBeliefNode);

    BeliefNode currBeliefNode;
    currBeliefNode = initBeliefNode;

    // 用一个栈结构保存belief路径
    stack<BeliefNode> beliefTree;
    vector<BeliefNode> beliefTreeStore;

    Graph G;

    // 迭代，达到targetPrecision时收敛
    while (epoch <= 5)
    {
        cout << "epoch: " << epoch << endl;
        // G初始化
        G.initGraph();
        cout << "length: " << G.NodeSet.size() << endl;
        // cout << G.NodeSet[1].action << endl;
        // cout << G.NodeSet[1].nodeNum << endl;
        // cout << G.NodeSet[1].nextEdge.obsEdge[0] << endl;
        beliefTree.push(currBeliefNode);
        beliefTreeStore.push_back(currBeliefNode);
        cout << "beliefTree.size(): " << beliefTree.size() << endl;
        cout << "beliefTreeStore.size: " << beliefTreeStore.size() << endl;

        // cout << "beliefTree.top().Ex1: " << beliefTree.top().Ex1 << endl;
        // cout << "beliefTree.top().Ey1: " << beliefTree.top().Ey1 << endl;
        // cout << "beliefTree.top().Ex2: " << beliefTree.top().Ex2 << endl;
        // cout << "beliefTree.top().Ey2: " << beliefTree.top().Ey2 << endl;


        // belief路径最后一个belief点向前更新G
        // while(!beliefTree.empty())
        // {
        //     // 获取st的栈顶元素并输出
		//     MCBackup(G, beliefTree.top(), 10);    
        //     cout << "length: " << G.NodeSet.size() << endl;
        //     cout << G.NodeSet[0].action << endl;
        //     cout << G.NodeSet[0].nodeNum << endl;
        //     cout << G.NodeSet[0].nextEdge.obsEdge[0] << endl;

        //     //弹出st的栈顶元素（不保存） 
		//     beliefTree.pop();       
	    // }

        for(auto iter = beliefTreeStore.rbegin(); iter != beliefTreeStore.rend(); iter++)
        {
            //cout << "Ex1" << *iter << endl;
            cout << "G: " << G.NodeSet.size() << endl;
            MCBackup(G, *iter, 20);    
        }

        
        lowerBound = LowerBound(initBeliefNode, G);
        cout << "lowerBound: " << lowerBound << endl;
        cout << "initBeleif upperBound" << upperBound(initBeliefNode, G) << endl;
        cout << endl;

        // sample一个新的belief
        // 这里从已有belief到新belief需要选取合适的a与o
        Belief b;
        BeliefNode nextBelief;
        BeliefNode bestNextBelief;
        double bestNextLowerBound = NegInf;
        for(int i = 0; i < 3; ++i)
        {
            nextBelief = b.nextBelief(currBeliefNode, act[i]);
            double NextLowerBound = upperBound(nextBelief, G);
            //cout << "upperBound: " << NextLowerBound << endl;
            if(NextLowerBound >= bestNextLowerBound)
            {
                bestNextLowerBound = NextLowerBound;
                bestNextBelief = nextBelief;
            }
        }
        
        currBeliefNode = bestNextBelief;
        epoch++;
        //cout << G.allNodes.size();
    }

    // 打印图G
    int Glength = G.NodeSet.size();
    //cout << "G.allNodes.size(): " << G.allNodes.size() << endl;

     for(int i = 0; i < Glength; ++i)
     {
        cout << "num: " << i << " action: " << G.NodeSet[i].action << 
            " edge: " << G.NodeSet[i].nextEdge.obsEdge[0] << " "
            << G.NodeSet[i].nextEdge.obsEdge[1] << " "
            << G.NodeSet[i].nextEdge.obsEdge[2] << endl;
    }

    G.GenerateGraph("policyGraph.txt");
}

// void ValueIteration::write(std::string filename)
// {
//     ofstream fp;
//     fp.open(filename.c_str());
//     if (!fp.is_open())
//     {
//         cerr << "Fail to open " << filename << "\n";
//         exit(EXIT_FAILURE);
//     }
//     fp << numStates << "\n";
//     for (long i = 0; i < numStates; i++)
//     {
//         fp << actions[i] << "\n";
//     }
//     fp.close();
// }

// void ValueIteration::read(std::string filename)
// {
//     ifstream fp;
//     fp.open(filename.c_str(), ios::in);
//     if (!fp.is_open())
//     {
//         cerr << "Fail to open " << filename << "\n";
//         exit(EXIT_FAILURE);
//     }

//     fp >> numStates;
//     actions.resize(numStates,0);
//     for (long i = 0; i < numStates; i++)
//     {
//         fp >> actions[i];
//     }
//     fp.close();
// }
