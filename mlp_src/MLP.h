#ifndef MLP_H
#define MLP_H

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

typedef struct{
     double t, c;
     int w;
}tCost;

// A structure that represents a solution
typedef struct{
     std::vector<int> route;
     std::vector<std::vector<tCost>> cost;
}tSolution;

class MLP{
     tSolution s, best, final;
     double **matrix;
     int dimension;
     std::vector<int> candidateList;
     Timer timer;

     void construction(),
          fillCost(),
          perturb();

     bool swap(),
          revert(),
          reinsert(int num);
         
     int random(int num);

     //-----===== Debugging functions =====-----

     void printSolution(tSolution &solution),
          printRoute(std::vector<int> &route);
     double getRealCost(int from, int to);

     //-----===============================-----

     public:
          MLP(double ***mPointer, int dimension);

          void printSolution(),
               printTimes(),
               printMatrix();

          double getCost(),
                 getRealCost();

          std::vector<double> *getTimes();
};

#endif // MLP_H
