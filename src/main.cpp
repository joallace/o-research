#include "readData.h"
#include "TSP.h"

int main(int argc, char** argv) {
    double ** matrizAdj; // matriz de adjacencia
    int dimension; // quantidade total de vertices
    
    readData(argc, argv, &dimension, &matrizAdj);

    TSP tsp = TSP(&matrizAdj, dimension);

    if(dimension < 16)
        tsp.printMatrix();

    std::cout << "\nTotal cost: " << tsp.getCost() << "\nReal cost: " << tsp.getRealCost() << std::endl;
    tsp.printSolution();


    return 0;
}