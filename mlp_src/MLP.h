#ifndef MLP_H
#define MLP_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Structures.h"
#include "Timer.h"

#define IMAX 10
#define SUBTOUR_SIZE 3
#define NEIGHBORLIST_SIZE 5
#define DEFAULT_NEIGHBORLIST {1, 2, 3, 4, 5}
//#define DEFAULT_NEIGHBORLIST {3,4,5}
#define LAST route.size()-1

class MLP{
     tSolution s, best, final;
     double **matrix;
     int dimension;
     std::vector<int> candidateList;
     Timer timer;
     //friend subCost subCost::operator+(const subCost& other) const;

     void construction(),
          fillCost(),
          perturb(),
          concatenate(tCost &s1, const tCost &s2, int s1_last, int s2_first);

     bool swap(),
          revert(),
          reinsert(int num);

     // tCost concatenate(tCost &s1, tCost &s2, int last, int first);
         
     int random(int num);

     //-----===== Debugging functions =====-----

     void printSolution(tSolution &solution),
          printRoute(std::vector<int> &route);

     double getSolutionCost(tSolution &solution);

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
