#ifndef BB_H
#define BB_H

#include <list>
#include "problem.h"
#include "hungarian.h"
#include "structures.h"
#include "tsp.h"

#define HUNGARIAN_INFINITY 999999999

class BB : public Problem{
    tSolution<double> s_;

    hungarian_problem_t p_;
    
    std::list<tNode> tree_;

    std::vector<std::vector<int>> subtours_;
    
    std::list<tNode>::iterator current_node;

    double upper_bound;
    double lower_bound;
    
    void vector_solve();
    int getSubtourIndex();

    void printAssingmentMatrix();

    public:
        BB(double ***matrix_pointer, int dimension);

        void printSolution();

        void printTimes();

        double getCost();
};

#endif // BB_H