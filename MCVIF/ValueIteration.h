#ifndef __VALUEITERATION_H
#define __VALUEITERATION_H


#include "Graph.h"
#include "Belief.h"
#include "Utils.h"
#include "Gauss.h"
#include "State.h"
#include "Belief.h"
#include "Graph.h"
#include "Simulate.h"



/**
   @class ValueIteration
   @brief Simple value iteration for MDPs.
*/

class ValueIteration
{
  public:
   //  ValueIteration(long numStates, long numActions, double discount): numStates(numStates), numActions(numActions), discount(discount) {};

   //  ValueIteration(long numActions, double discount): numActions(numActions), discount(discount) {};

   //  void doValueIteration(std::vector<std::vector<double> >& rewardMatrix, std::vector<std::vector<std::vector<std::pair<long,double> > > >& transMatrix, double targetPrecision, long displayInterval);

   //  std::vector<double> values;
   //  std::vector<long> actions;
   void MCBackup(Graph &G, BeliefNode beliefNode, int N);
   void show();

   void doIteration();

   double upperBound(BeliefNode belief, Graph G);
   
   double LowerBound(BeliefNode belief, Graph G);

   //  /**
   //     Write out the policy \a filename
   //  */
   //  static void write(std::string filename);

   //  /**
   //     Reads in what has be written out with \a write
   //  */
   //  void read(std::string filename);

    long numStates;
    int numActions = 3;
    double discount;

};

#endif
