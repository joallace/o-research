#ifndef TSP_H
#define TSP_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <sys/timeb.h>
#include <sys/resource.h>

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

class TSP{
     double **matrix, cost, bestCost;
     int dimension;
     std::vector<int> route, bestRoute, candidateList;
    
     void subtour(),
          initialRoute(),
          peturb();

     bool swap(),
          revert(),
          reinsert(int num);
         
     int random(int num);

     double cpuTime();

     public:
          TSP(double ***mPointer, int dimension);

          void printSolution(),
               printFinalSolution(),
               printMatrix();

          double getCost(),
                 getRealCost(),
                 getRealFinalCost();
};

#endif // TSP_H
