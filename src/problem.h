#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "timer.h"

#define SUBTOUR_SIZE 3
#define NEIGHBORLIST_SIZE 5
#define DEFAULT_NEIGHBORLIST {1, 2, 3, 4, 5}

class Problem{
     protected:
          double **matrix_;
          int dimension_;
          std::vector<int> candidate_list_;
          Timer timer_;

          virtual void perturb() = 0;

          virtual bool swap() = 0,
                       revert() = 0,
                       reinsert(int num) = 0;
          
          int random(int num) const;

          void printRoute(std::vector<int> &route);

     public:
          Problem(double ***matrix_pointer, int dimension);

          void printTimes(),
               printMatrix();
          
          virtual void printSolution() = 0;

          virtual double getCost() = 0,
                         getRealCost() = 0;

          std::vector<double>* getTimes();
          int64_t* getTimerPointer();
};

#endif // PROBLEM_H
