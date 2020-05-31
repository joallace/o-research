#include "problem.h"

Problem::Problem(double ***matrix_pointer, int dimension){
    timer_ = Timer();
    matrix_ = *matrix_pointer;
    dimension_ = dimension;
}

// Just a function that returns a random number from [1, num]
int Problem::random(int num) const{
    return (rand()%num)+1;
}

void Problem::printMatrix(){
    std::cout << "Dimension: " << dimension_ << "\n\n";
    for(int i = 0; i < dimension_; i++){
        for(int j = 0; j < dimension_; j++){
            char endian = ((j+1)==dimension_) ? '\n' : ' ';
            std::cout << matrix_[i][j] << endian;
        }
    }
}

void Problem::printTimes(){
    std::cout << "Total time: " << timer_.getTotalTime() << " (s)\n"
              << "| Construction execution time: " << timer_.getConstructionTime() << " (s)\n"
              << "| Swap execution time: " << timer_.getSwapTime() << " (s)\n"
              << "| 2-opt execution time: " << timer_.getRevertTime() << " (s)\n"
              << "| Or-opt execution time: " << timer_.getReinsertion1Time() << " (s)\n"
              << "| Or-opt2 execution time: " << timer_.getReinsertion2Time() << " (s)\n"
              << "| Or-opt3 execution time: " << timer_.getReinsertion3Time() << " (s)\n\n";
}

int64_t* Problem::getTimerPointer(){
    return timer_.getPointer();
}

void Problem::printRoute(std::vector<int> &route){
    for(int i = 0; i < route.size(); i++)
        printf("%d%s", route[i]+1, i+1 == route.size()?"\n":", ");
}