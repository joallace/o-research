#include "readData.h"
#include "TSP.h"

double ** matrizAdj; // matriz de adjacencia
int dimension; // quantidade total de vertices

int main(int argc, char** argv) {
    readData(argc, argv, &dimension, &matrizAdj);
        srand(time(NULL));
    TSP tsp = TSP(&matrizAdj, dimension);
    tsp.printMatrix();

    std::cout << "\nTotal cost: " << tsp.getCost() << "\nReal cost: " << tsp.getRealCost() << std::endl;
    tsp.printSolution();


    return 0;
}