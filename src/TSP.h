#ifndef TSP_H
#define TSP_H

#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

#define SUBTOUR_SIZE 3
#define INFINITY std::numeric_limits<double>::infinity()

typedef struct move{
        int i, j;
        double delta;
        bool operator<(const move& other) const{
            return delta < other.delta;
        }
}tMove;

class TSP{
    double **matrix, cost;
    int dimension;
    std::vector<int> route, candidateList;
    
    void subtour(),
         initialRoute(),
         swap(),
         revert(),
         reinsert(int num);

    int random(int num);

    public:
        TSP(double ***mPointer, int dimension);

        void printSolution(),
             printMatrix();

        double getCost(),
               getRealCost(),
               getRealCost(int size);
};

#endif // TSP_H
