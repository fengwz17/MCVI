#ifndef __GRAPH_H
#define __GRAPH_H


// #include "Action.h"
#include "Utils.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

class Graph
{
    public:
        struct Node;

        struct Edge
        {
            // <observe, Node>
            //std::map<int, Node*> obsEdge;
            // <ob, nodeNum>
            map<int, long> obsEdge;
        };

        // struct Edge operator =(const struct Edge &a)
        // {
        //     return a;
        // };

        struct Node
        {
            long nodeNum;
            double action;
            Edge nextEdge;
        };

        

        // <编号， 顶点>
        map<long, Node> NodeSet;

        void initGraph()
        {
            for(int i = 0; i < 3; ++i)
            {
                Node newNode;
                Edge newEdge;
                
                newNode.nodeNum = i;
                newNode.action = act[i];
                
                // 初始图的节点，通过观测边连接到自己
                newEdge.obsEdge[0] = i;
                newEdge.obsEdge[1] = i;
                newEdge.obsEdge[2] = i;

                newNode.nextEdge = newEdge;

                this->NodeSet[i] = newNode;
                //this->allNodes.push_back(newNode);
            }
            int length = this->NodeSet.size();
            if(length > 3)
            {
                for(int i = 3; i < length; i++)
                {
                    this->NodeSet.erase(this->NodeSet.find(i));
                }
            }
        }

        void GenerateGraph(string filename)
        {
            ofstream f;
            f.open(filename.c_str());
            string result = "";
            result += "digraph G {\n";
            result += "node [shape = \"ellipse\", sides = 4, color = \"blue\"] \n";
            for(int i = 0; i < this->NodeSet.size(); i++)
            {
                result += "action" + to_string(i) + "[label = \"" + 
                    actionTrans(this->NodeSet[i].action) + to_string(i) + "\"] \n"; 
            }
            result += "edge [color = \"red\", decorate = false] \n";
            for(int i = 0; i < this->NodeSet.size(); i++)
            {   
                for(int j = 0; j < 3; j++)
                {
                    result += "action" + to_string(i) + " -> " + 
                        "action" + to_string(this->NodeSet[i].nextEdge.obsEdge[j]) +
                            " [style = \"solid\", color = \"red\", label = \""
                                + obsTrans(j) + "\"] \n";
                }
               
            }

            result += "}\n";
           
            f << result;    
            f.close();        
        }

        string actionTrans(double a)
        {
            if(a > 0)
                return "turn left";
            else if(a == 0)
                return "go ahead";
            else
                return "turn right";
        }

        string obsTrans(int o)
        {
            if(o == 0)
                return "no-detection";
            if(o == 1)
                return "left";
            if(o == 2)
                return "right";
        }
        

};


#endif