#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

// A structure that stores the cost from a certain move involving i and j
template <typename T>
struct tMove{
    int i, j;
    T cost;
    bool operator<(const tMove& other) const{
        return cost < other.cost;
    }
};

// A structure to store the w, t and c costs (MLP exclusive)
struct tCost{
    int w;
    double t;
    double c;
};

// A structure that represents a solution
template <typename T>
struct tSolution{
    std::vector<int> route;
    T cost;
};

#endif // STRUCTURES_H