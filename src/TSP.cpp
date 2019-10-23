#include "TSP.h"

TSP::TSP(double ***mPointer, int dimension){
    //Setting up
    timer = Timer();
    matrix = *mPointer;
    this->dimension = dimension;
    bestCost = INFINITY;
    
    //Defining variables
    int i, i_ILS = dimension>=150 ? dimension/2 : dimension;
    std::vector<char> neighborList, defaultNeighborList;

    //Defining the defaultNeighborList
    for(i = 0; i < NEIGHBORLIST_SIZE; i++)
        defaultNeighborList.push_back(i+1);

    //GILS
    for(int iMax = 0; iMax < IMAX; iMax++){
        cost = 0;

        //Filling up the candidate list
        for(i = 0; i < dimension; i++)
            candidateList.push_back(i);
        
        timer.setTime(0);
        //Creating a initial subtour
        subtour();

        //Filling up the solution vector feasibly
        initialRoute();
        timer.setTime(0);

        //RVND
        for(int iILS = 0; iILS < i_ILS; iILS++){
            neighborList = defaultNeighborList;
            while(!neighborList.empty()){
                i = random(neighborList.size())-1;
                switch(neighborList[i]){
                    case 1:
                        if(!swap())
                            neighborList.erase(neighborList.begin() + i);
                        else if(neighborList.size() != NEIGHBORLIST_SIZE)
                            neighborList = defaultNeighborList;
                        break;

                    case 2:
                        if(!revert())
                            neighborList.erase(neighborList.begin() + i);
                        
                        else if(neighborList.size() != NEIGHBORLIST_SIZE)
                            neighborList = defaultNeighborList;
                        break;

                    case 3:
                        if(!reinsert(1))
                            neighborList.erase(neighborList.begin() + i);
                        
                        else if(neighborList.size() != NEIGHBORLIST_SIZE)
                            neighborList = defaultNeighborList;
                        break;

                    case 4:
                        if(!reinsert(2))
                            neighborList.erase(neighborList.begin() + i);
                        else if(neighborList.size() != NEIGHBORLIST_SIZE)
                            neighborList = defaultNeighborList;
                        break;

                    case 5:
                        if(!reinsert(3))
                            neighborList.erase(neighborList.begin() + i);
                        else if(neighborList.size() != NEIGHBORLIST_SIZE)
                            neighborList = defaultNeighborList;
                        break;
                }
            }
            if(bestCost > cost){
                bestCost = cost;
                bestRoute = route;
                iILS = 0;
            }
            else{
                cost = bestCost;
                route = bestRoute;
            }

            pertub();
        }

        route.clear();
    }

    timer.stop();
}

//Just a function that returns a random number from [1, num]
int TSP::random(int num){
    return (rand()%num)+1;
}

void TSP::subtour(){
    //Obtaining an initial item randomly
    int first = random(dimension-1);

    //Inserting it into the solution and removing it from the candidate list
    route.push_back(first);
    candidateList.erase(candidateList.begin() + first);

    //Inserting random items from the candidate list into the solution
    for(int i = 0; i < SUBTOUR_SIZE; i++){
        int j = random(candidateList.size()) - 1;
        cost += matrix[route[i]][candidateList[j]];
        route.push_back(candidateList[j]);
        candidateList.erase(candidateList.begin() + j);  
    }

    //Finishing the Hamiltonian cycle
    route.push_back(first);
    cost += matrix[route[SUBTOUR_SIZE]][route[SUBTOUR_SIZE+1]];
}

void TSP::initialRoute(){
    tMove* nextNode;
    
    //Repeating until a feasible initial solution is found
    while (!candidateList.empty()){
        //Calculating the insertion cost of each one of the remaining candidates
        std::vector <tMove*> costVector((route.size()-2) * candidateList.size());

        for(int i = 1, k = 0; i < route.size()-1; i++){
            for(int j = 0; j < candidateList.size(); j++){
                //Allocating a cost node
                nextNode = (tMove*)malloc(sizeof(tMove));

                //Assigning the indexes to the node members
                nextNode->i = j;
                nextNode->j = i;

                //Calculating the insertion cost and inserting the node into the vector
                nextNode->delta =  matrix[route[i]][candidateList[j]] 
                                  +matrix[candidateList[j]][route[i+1]]
                                  -matrix[route[i]][route[i+1]];

                costVector[k++] = nextNode;
            }
        }
    
        //Sorting the costVector
        std::sort(costVector.begin(), costVector.end());
        
        //Obtaining an item in a random interval of the costVector
        nextNode = costVector[random((int)random(10)/10.0 * (costVector.size()- 1))];

        //Inserting the item into the solution and removing it from the candidate list
        route.insert(route.begin() + (nextNode->j) + 1, candidateList[nextNode->i]);
        cost += nextNode->delta;
        candidateList.erase(candidateList.begin() + nextNode->i);
    }
}

bool TSP::swap(){
    //Criando um movimento de swap com delta infito, a fim de nao pegar lixo de memoria, e nao bugar o if
    tMove bestSwap = {0, 0, INFINITY};
    double delta;

    timer.setTime(1);
    //Repeating until the swap with lowest delta is found
    for(int i = 1; i < dimension - 2; i++){
        for(int j = i + 1; j < dimension - 1; j++){
            if(j-i != 1)
                delta =  matrix[route[i]][route[j-1]]
                        +matrix[route[i]][route[j+1]]
                        +matrix[route[j]][route[i-1]]
                        +matrix[route[j]][route[i+1]]
                        -matrix[route[i]][route[i-1]]
                        -matrix[route[i]][route[i+1]]
                        -matrix[route[j]][route[j-1]]
                        -matrix[route[j]][route[j+1]];
            else
                delta =  matrix[route[i]][route[j+1]]
                        +matrix[route[j]][route[i-1]]
                        -matrix[route[i]][route[i-1]]
                        -matrix[route[j]][route[j+1]];
                 
            
            if(bestSwap.delta > delta){
                bestSwap.i = i;
                bestSwap.j = j;
                bestSwap.delta = delta;
            }
        }
    }

    //Making the swap in the route and inserting the delta in the cost
    if(bestSwap.delta < 0){
        cost = cost + bestSwap.delta;
        std::swap(route[bestSwap.i], route[bestSwap.j]);
        timer.setTime(1);
        return true;
    }

    timer.setTime(1);
    return false;
}

bool TSP::revert(){
    tMove bestReversion = {0, 0, INFINITY};
    double delta;

    timer.setTime(2);
    for(int i = 1; i < dimension - 2; i++){
        for(int j = i + 2; j < dimension - 1; j++){
            delta =  matrix[route[i]][route[j+1]]
                    +matrix[route[j]][route[i-1]]
                    -matrix[route[i]][route[i-1]]
                    -matrix[route[j]][route[j+1]];
            
            if(bestReversion.delta > delta){
                bestReversion.i = i;
                bestReversion.j = j;
                bestReversion.delta = delta;
            }
        }
    } 

    if(bestReversion.delta < 0){
        cost = cost + bestReversion.delta;
        std::reverse(route.begin() + bestReversion.i, route.begin() + bestReversion.j+1);
        timer.setTime(2);
        return true;
    }

    timer.setTime(2);
    return false;
}

bool TSP::reinsert(int num){
    tMove bestReinsertion = {0, 0, INFINITY};
    double delta;

    timer.setTime(2+num);
    for(int i = 1; i < dimension - (num-1); i++){
        for(int j = 0; j < dimension - num; j++){   //O j comeca em zero pois o i sera inserido numa posicao a frente do j
            //Checando se ele esta dentro do intervalo que sera movimentado
            if(j+1 == i){
                j += num;
                continue;
            }

            delta =  matrix[route[j]][route[i]]
                    +matrix[route[i+(num-1)]][route[j+1]]
                    +matrix[route[i-1]][route[i+num]]
                    -matrix[route[i-1]][route[i]]
                    -matrix[route[i+(num-1)]][route[i+num]]
                    -matrix[route[j]][route[j+1]];
            
            if(bestReinsertion.delta > delta){
                bestReinsertion.i = i;
                bestReinsertion.j = j;
                bestReinsertion.delta = delta;
            }
        }
    }
    
    if(bestReinsertion.delta < 0){
        cost = cost + bestReinsertion.delta;

        std::vector<int> subroute(route.begin() + bestReinsertion.i, route.begin() + bestReinsertion.i + num);
        route.erase(route.begin() + bestReinsertion.i, route.begin() + bestReinsertion.i + num);

        if(bestReinsertion.i > bestReinsertion.j)
            route.insert(route.begin() + (bestReinsertion.j+1), subroute.begin(), subroute.end());
        else
            route.insert(route.begin() + bestReinsertion.j-(num-1), subroute.begin(), subroute.end());
        
        timer.setTime(2+num);
        return true;
    }

    timer.setTime(2+num);
    return false;
}

void TSP::pertub(){
    int iSize = random(ceil(dimension/10.0)-1),    //min = 1 & max = dimension/10 - 1
        jSize = random(ceil(dimension/10.0)-1);

    int i = random(dimension - (iSize+jSize+2)),
        j = random(dimension - (i+iSize+jSize+1)) + (i+iSize);

    std::vector<int> subroute1(route.begin() + i, route.begin() + i + iSize + 1),
                     subroute2(route.begin() + j, route.begin() + j + jSize + 1);

    cost -=  matrix[route[i-1]][route[i]]
            +matrix[route[i+iSize]][route[i+iSize+1]]
            +((i+iSize+1 == j)? 0 : matrix[route[j-1]][route[j]])   //Checks if the subsequences are adjacent
            +matrix[route[j+jSize]][route[j+jSize+1]];

    route.erase(route.begin() + j, route.begin() + j + jSize + 1);
    route.erase(route.begin() + i, route.begin() + i + iSize + 1);
    route.insert(route.begin() + i, subroute2.begin(), subroute2.end());
    route.insert(route.begin() + j + (jSize - iSize), subroute1.begin(), subroute1.end());

    cost +=  matrix[route[i-1]][route[i]]
            +matrix[route[i+jSize]][route[i+jSize+1]]
            +((i+iSize+1 == j)? 0 : matrix[route[j + (jSize-iSize)-1]][route[j + (jSize-iSize)]])
            +matrix[route[j+jSize]][route[j+jSize+1]];
}

void TSP::printSolution(){
    for(int i = 0; i < bestRoute.size(); i++)
        printf("%d%s", bestRoute[i]+1, i+1 == bestRoute.size()?"\n":", ");
}

void TSP::printTimes(){
    std::cout << "Tempo medio de execucao da SI: " << timer.getTotalTime() << " (s)\n";
    std::cout << "Tempo medio de execucao da troca: " << timer.getSwapTime() << " (s)\n";
    std::cout << "Tempo medio de execucao do Or-opt: " << timer.getReinsertion1Time() << " (s)\n";
    std::cout << "Tempo medio de execucao do Or-opt2: " << timer.getReinsertion2Time() << " (s)\n";
    std::cout << "Tempo medio de execucao do Or-opt3: " << timer.getReinsertion3Time() << " (s)\n";
    std::cout << "Tempo medio de execucao do 2-opt: " << timer.getRevertTime() << " (s)\n\n";
}

double TSP::getCost(){
    return bestCost;
}

double TSP::getRealCost(){
    double sum = 0;

    for(int i = 0; i < dimension; i++){
        sum+= matrix[bestRoute[i]][bestRoute[i+1]];
    }
    
    return sum;
}

void TSP::printMatrix(){
    printf("Dimension: %d\n", dimension);
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            char endian = ((j+1)==dimension) ? '\n' : ' ';
            std::cout <<  matrix[i][j] << endian;
        }
    }
}

//------------============ Debugging functions ============------------

void TSP::printCurrentSolution(){
    for(int i = 0; i < route.size(); i++)
        printf("%d%s", route[i]+1, i+1 == route.size()?"\n":", ");
}

double TSP::getCurrentRealCost(){
    double sum = 0;

    for(int i = 0; i < dimension; i++){
        sum+= matrix[route[i]][route[i+1]];
    }
    
    return sum;
}

//------------============================================------------