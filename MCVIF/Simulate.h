#include "Utils.h"
#include "State.h"
#include "Graph.h"
#include "Gauss.h"

double simulate(Graph G, Graph::Node startNode, State startState, int L, double gamma, bool flag);
int observe(State* s, bool flag);