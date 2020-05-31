#ifndef TSP_H
#define TSP_H

#include "problem.h"
#include "structures.h"

#define TSP_IMAX 50

class TSP : public Problem{
     tSolution<double> s_, best_, final_;

     void perturb(),
          subtour(),
          initialRoute();

     bool swap(),
          revert(),
          reinsert(int num);

     double getSolutionCost(tSolution<double> &solution);

     public:
          TSP(double ***mPointer, int dimension);

          double getCost(),
                 getRealCost();

          void printSolution();
};

#endif // TSP_H
