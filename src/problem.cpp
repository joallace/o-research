#include "include/problem.h"

Problem::Problem(double ***matrix_pointer, int dimension){
    timer_ = Timer();
    matrix_ = *matrix_pointer;
    dimension_ = dimension;
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

int64_t* Problem::getTimerPointer(){
    return timer_.getPointer();
}

void Problem::printRoute(std::vector<int> &route){
    for(int i = 0; i < route.size(); i++)
        printf("%d%s", route[i]+1, i+1 == route.size()?"\n":", ");
}