#ifndef STRUCTURES_H
#define STRUCTURES_H

// A structure to store the w, t and c costs
struct tCost{
     int w;
     double t;
     double c;
};

// A structure that stores the cost from a certain move involving i and j
struct tMove{
     int i, j;
     tCost cost;
     bool operator<(const tMove& other) const{
          return cost.c < other.cost.c;
     }
};

// A structure that represents a solution
struct tSolution{
     std::vector<int> route;
     std::vector<std::vector<tCost>> cost;
};

#endif // STRUCTURES_H