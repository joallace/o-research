#ifndef MLP_H
#define MLP_H

#include "metaheuristic_problem.h"
#include "structures.h"

#define MLP_IMAX 10
#define LAST route.size()-1

class MLP : public MetaheuristicProblem{
    tSolution<std::vector<std::vector<tCost>>> s_, best_, final_;

    void perturb(),
         construction(),
         fillCost(),
         computeCost(int first, int last),
         concatenate(tCost &s1, const tCost &s2, int s1_last, int s2_first) const;

    bool swap(),
         revert(),
         reinsert(int num);

    //-----===== Debugging functions =====-----

    double getSolutionCost(tSolution<std::vector<std::vector<tCost>>> &solution);

    //-----===============================-----

    public:
        MLP(double ***matrix_pointer, int dimension);

        double getCost(),
               getRealCost();

        void printSolution();
};

#endif // MLP_H
