#ifndef TSP_H
#define TSP_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Timer.h"

#define SUBTOUR_SIZE 3
#define NEIGHBORLIST_SIZE 5
#define IMAX 50

//A structure that stores the cost from a certain move involving i and j
typedef struct move{
     int i, j;
     double delta;
     bool operator<(const move& other) const{
          return delta < other.delta;
     }
}tMove;

typedef struct{
     double cost;
     std::vector<int> route;
}tSolution;

class TSP{
     tSolution s, best, final;
     double **matrix;
     int dimension;
     std::vector<int> candidateList;
     Timer timer;

     void subtour(),
          initialRoute(),
          pertub();

     bool swap(),
          revert(),
          reinsert(int num);
         
     int random(int num);

     //-----===== Debugging functions =====-----

     void printCurrentSolution(bool returnBest);   //Insert true to get the bestRoute or bestCost
     double getCurrentRealCost(bool returnBest);

     //-----===============================-----

     public:
          TSP(double ***mPointer, int dimension);

          void printSolution(),
               printTimes(),
               printMatrix();

          double getCost(),
                 getRealCost();
};

#endif // TSP_H
