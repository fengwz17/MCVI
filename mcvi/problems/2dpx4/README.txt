There are ownship and intruder. Ownship needs to make optimal decision 
to avoid collision. To simplify the problem, decision action is 2D and 
aircraft can only descend, ascend and level off.

The ownship does not know intruder's exact location, but can gather information 
from observations:
ObsNothing,Left, Right.

The horizon velocity is constant. The step length is 1. Some noise could be 
added later.

Collision gives a cost. Turn left and turn right give a movement cost of -1.
Level off gives no cost to encourage the reduction of irrelevant actions.
