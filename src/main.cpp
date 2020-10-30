#include "include/read_data.h"
#include "include/mlp.h"
#include "include/tsp.h"
#include "include/bb.h"

#include <cstring>
#include <ctime>

double **matrix; // Adjacency matrix
int dimension; // Total vertex number 

struct args{
    int instance_index = 0;
    char mode = 0;
    bool benchmark = false; 
};

args arguments;

Problem* newProblem(){
    switch(arguments.mode){
            case 'm':
                return new MLP(&matrix, dimension);
                break;

            case 't':
                return new TSP(&matrix, dimension);
                break;

            case 'b':
                return new BB(&matrix, dimension);
                break;
            
            default:
                std::cout << "Specify the problem type with --mlp or --tsp or --bb\n";
                exit(1);
                break;
    }
}

void benchmark(){
    double cost_mean = 0, 
           time_mean[7] = {};

    int64_t *current_time;

    for(int i = 1; i <= 10; i++){
        Problem *p = newProblem();

        cost_mean += p->getCost();
        current_time = p->getTimerPointer();

        for(int j = 0; j < 7; j++)
            time_mean[j] += current_time[j];

        std::cout << "ITERATION " << i << " COST: " << p->getCost() << "\n";
        delete p;
    }

    std::cout << "\n--------======== Averages ========---------\n"
                << "Average cost: " << cost_mean/10 << "\n"
                << "Average execution time: " << time_mean[6]/10000000000 << " (s)\n"
                << "| Construction execution time: " << time_mean[0]/10000000000 << " (s)\n"
                << "| Swap execution time: " << time_mean[1]/10000000000 << " (s)\n"
                << "| 2-opt execution time: " << time_mean[2]/10000000000 << " (s)\n"
                << "| Or-opt execution time: " << time_mean[3]/10000000000 << " (s)\n"
                << "| Or-opt2 execution time: " << time_mean[4]/10000000000 << " (s)\n"
                << "| Or-opt3 execution time: " << time_mean[5]/10000000000 << " (s)\n\n";
}

void argParse(int argc, char** argv){
    size_t length;

    if (argc < 3) {
        std::cerr << "\nERROR: Missing parameters\n"
                  << " ./solver [Instance] --mode -[optional flags]\n";
        exit(1);
    }

    if (argc > 4) {
        std::cerr << "\nERROR: Too many parameters\n"
                  << " ./solver [Instance] --mode -[optional flags]\n";
        exit(1);
    }

    for(int i = 1; i < argc; i++){
        if(strstr(argv[i], ".tsp") != NULL){
            arguments.instance_index = i;
            continue;
        }

        if(strstr(argv[i], "--") == argv[i]){
            arguments.mode = argv[i][2];
            continue;
        }

        if(!strcmp(argv[i], "-b")){
            arguments.benchmark = true;
        }
    }

    if(!arguments.instance_index){
        std::cerr << "\nERROR: Invalid instance file\n";
        exit(1);
    }
}

int main(int argc, char** argv) {
    argParse(argc, argv);

    readData(argv[arguments.instance_index], &dimension, &matrix);
    
    srand(time(NULL));

    std::cout << std::endl;

    if(arguments.benchmark) // Benchmark mode
        benchmark();
    else{ 
        Problem* p = newProblem();

        if(dimension < 16){
            p->printMatrix();
            std::cout << std::endl;
            p->printSolution();
        }

        std::cout << "Total cost: " << p->getCost() << "\n\n";

        p->printTimes();
    }

    return 0;
}