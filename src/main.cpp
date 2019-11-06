#include "readData.h"
#include "TSP.h"

int main(int argc, char** argv) {
    double ** matrizAdj; // matriz de adjacencia
    int dimension; // quantidade total de vertices
    
    readData(argc, argv, &dimension, &matrizAdj);
    
    srand(time(NULL));

    TSP tsp = TSP(&matrizAdj, dimension);

    if(dimension < 16)
        tsp.printMatrix();

    std::cout << "\nTotal cost: " << tsp.getCost() << std::endl;

    //tsp.printSolution();

    std::cout << "\n\n";

    tsp.printTimes();

    return 0;
}