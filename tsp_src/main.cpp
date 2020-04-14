#include "read_data.h"
#include "tsp.h"

int main(int argc, char** argv) {
    double ** matrizAdj; // Adjacency matrix
    int dimension; // Total vertex number 
    
    readData(argc, argv, &dimension, &matrizAdj);
    
    srand(time(NULL));

    std::cout << std::endl;

    if(argc > 2 && argv[2][1] == 'b'){ // Benchmark mode
        double costMean = 0, 
               timeMean[7] = {};
        std::vector<double> *currentTime;

        for(int i = 1; i <= 10; i++){
            TSP *tsp = new TSP(&matrizAdj, dimension);
            costMean += tsp->getCost()/10;
            currentTime = tsp->getTimes();

            for(int j = 0; j < 7; j++)
                timeMean[j] += (*currentTime)[j];

            std::cout << "Iteration " << i << " cost: " << tsp->getCost() << "\n";
            delete currentTime;
            delete tsp;
        }

        std::cout << "\n--------======== Averages ========---------\n"
                  << "Average cost: " << costMean << "\n"
                  << "Total execution time: " << timeMean[0]/10 << " (s)\n"
                  << "| Construction execution time: " << timeMean[1]/10 << " (s)\n"
                  << "| Swap execution time: " << timeMean[2]/10 << " (s)\n"
                  << "| 2-opt execution time: " << timeMean[3]/10 << " (s)\n"
                  << "| Or-opt execution time: " << timeMean[4]/10 << " (s)\n"
                  << "| Or-opt2 execution time: " << timeMean[5]/10 << " (s)\n"
                  << "| Or-opt3 execution time: " << timeMean[6]/10 << " (s)\n\n";
    } 
    else{ 
        TSP tsp = TSP(&matrizAdj, dimension);

        if(dimension < 16){
            tsp.printMatrix();
            std::cout << std::endl;
            tsp.printSolution();
        }

        std::cout << "Total cost: " << tsp.getCost() << " (" << tsp.getRealCost() << ")\n\n";

        tsp.printTimes();
    }

    return 0;
}