#ifndef STRUCTURES_H
#define STRUCTURES_H

struct tCost{
     int w;
     double t;
     double c;

     // int first,
     //     last;
     
     // tCost operator+(const tCost& other) const{
     //      return subCost{
     //           w + other.w,
     //           t + matrix[s.route[last]][s.route[other.first]] + other.t,
     //           c + other.w*(t + matrix[s.route[last]][s.route[other.first]]) + other.c,
     //           first,
     //           other.last,
     //      };
     // }
};

// A structure that stores the cost from a certain move involving i and j
struct tMove{
     int i, j;
     tCost delta;
     bool operator<(const tMove& other) const{
          return delta.c < other.delta.c;
     }
};

// A structure that represents a solution
struct tSolution{
     std::vector<int> route;
     std::vector<std::vector<tCost>> cost;
};

#endif // STRUCTURES_H