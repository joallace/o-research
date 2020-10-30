#include "include/metaheuristic_problem.h"

MetaheuristicProblem::MetaheuristicProblem(double ***matrix_pointer, int dimension):
Problem(matrix_pointer, dimension){}

// Just a function that returns a random number from [1, num]
int MetaheuristicProblem::random(int num) const{
    return (rand()%num)+1;
}

void MetaheuristicProblem::printTimes(){
    std::cout << "Total time: " << timer_.getTotalTime() << " (s)\n"
              << "| Construction execution time: " << timer_.getConstructionTime() << " (s)\n"
              << "| Swap execution time: " << timer_.getSwapTime() << " (s)\n"
              << "| 2-opt execution time: " << timer_.getRevertTime() << " (s)\n"
              << "| Or-opt execution time: " << timer_.getReinsertion1Time() << " (s)\n"
              << "| Or-opt2 execution time: " << timer_.getReinsertion2Time() << " (s)\n"
              << "| Or-opt3 execution time: " << timer_.getReinsertion3Time() << " (s)\n\n";
}
