#ifndef TSP_H
#define TSP_H

#include "metaheuristic_problem.h"
#include "structures.h"

#define TSP_IMAX 50

class TSP : public MetaheuristicProblem{
    tSolution<double> s_, best_, final_;

    void perturb(),
         subtour(),
         initialRoute();

    bool swap(),
         revert(),
         reinsert(int num);

    double getSolutionCost(tSolution<double> &solution);

    public:
        TSP(double ***matrix_pointer, int dimension, int iterations = TSP_IMAX);

        tSolution<double> getSolution();

        double getCost(),
               getRealCost();

        void printSolution();
};

#endif // TSP_H
