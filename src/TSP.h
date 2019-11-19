#ifndef TSP_H
#define TSP_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Timer.h"

#define SUBTOUR_SIZE 3
#define NEIGHBORLIST_SIZE 5
#define DEFAULT_NEIGHBORLIST {1, 2, 3, 4, 5}
#define IMAX 50

// A structure that stores the cost from a certain move involving i and j
typedef struct move{
     int i, j;
     double delta;
     bool operator<(const move& other) const{
          return delta < other.delta;
     }
}tMove;

// A structure that represents a solution
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
          perturb();

     bool swap(),
          revert(),
          reinsert(int num);
         
     int random(int num);

     //-----===== Debugging functions =====-----

     void printCurrentSolution(bool returnBest);   // Insert true to get the bestRoute
     double getCurrentRealCost(bool returnBest);  // Insert true to get the bestCost

     //-----===============================-----

     public:
          TSP(double ***mPointer, int dimension);

          void printSolution(),
               printTimes(),
               printMatrix();

          double getCost(),
                 getRealCost();

          std::vector<double> *getTimes();
};

#endif // TSP_H
