#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "timer.h"

class Problem{
    protected:
        double **matrix_;
        int dimension_;
        Timer timer_;

        void printRoute(std::vector<int> &route);

    public:
        Problem(double ***matrix_pointer, int dimension);

        void printMatrix();

        virtual void printTimes() = 0;
        
        virtual void printSolution() = 0;

        virtual double getCost() = 0;

        std::vector<double>* getTimes();
        
        int64_t* getTimerPointer();
};

#endif // PROBLEM_H
