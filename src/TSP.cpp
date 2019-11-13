#include "TSP.h"

TSP::TSP(double ***mPointer, int dimension){
    // Setting up
    timer = Timer();
    matrix = *mPointer;
    this->dimension = dimension;
    final.cost = INFINITY;
    
    // Defining variables
    int i, i_ILS = dimension>=150 ? dimension/2 : dimension;
    std::vector<char> neighborList, defaultNeighborList = {1, 2, 3, 4, 5};

    // GILS
    for(int iMax = 0; iMax < IMAX; iMax++){
        s.cost = 0;

        // Filling up the candidate list
        for(i = 0; i < dimension; i++)
            candidateList.push_back(i);


        //---=== Construction ===---
        timer.setTime(0);
        
        subtour(); // Creating a initial subtour
        initialRoute(); // Filling up the solution vector feasibly

        timer.setTime(0);
        //---====================---

        best.cost = INFINITY;

        // RVND
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
            if(s.cost < best.cost){
                best.cost = s.cost;
                best.route = s.route;
                iILS = 0;
            }
            else{
                s.cost = best.cost;
                s.route = best.route;
            }
            perturb();
        }
        if(best.cost < final.cost){
            final.cost = best.cost;
            final.route = best.route;
        }
        s.route.clear();
    }
    timer.stop();
}

//Just a function that returns a random number from [1, num]
inline int TSP::random(int num){
    return (rand()%num)+1;
}

void TSP::subtour(){
    //Obtaining an initial item randomly
    int first = random(dimension-1);

    //Inserting it into the solution and removing it from the candidate list
    s.route.push_back(first);
    candidateList.erase(candidateList.begin() + first);

    //Inserting random items from the candidate list into the solution
    for(int i = 0; i < SUBTOUR_SIZE; i++){
        int j = random(candidateList.size()) - 1;
        s.cost += matrix[s.route[i]][candidateList[j]];
        s.route.push_back(candidateList[j]);
        candidateList.erase(candidateList.begin() + j);  
    }

    //Finishing the Hamiltonian cycle
    s.route.push_back(first);
    s.cost += matrix[s.route[SUBTOUR_SIZE]][s.route[SUBTOUR_SIZE+1]];
}

void TSP::initialRoute(){    
    //Repeating until a feasible initial solution is found
    while (!candidateList.empty()){
        //Calculating the insertion cost of each one of the remaining candidates
        tMove costVector[(s.route.size()-2) * candidateList.size()];

        for(int i = 1, k = 0; i < s.route.size()-1; i++){
            for(int j = 0; j < candidateList.size(); j++){
                //Assigning the indexes to the node members
                costVector[k].i = j;
                costVector[k].j = i;

                //Calculating the insertion cost and inserting the node into the vector
                costVector[k++].delta =  matrix[s.route[i]][candidateList[j]] 
                                        +matrix[candidateList[j]][s.route[i+1]]
                                        -matrix[s.route[i]][s.route[i+1]];
            }
        }
    
        //Sorting the costVector
        std::sort(&costVector[0], &costVector[(s.route.size()-2) * candidateList.size()]);
        
        //Obtaining an item in a random interval of the costVector
        tMove* nextNode = &costVector[random((int)random(10)/10.0 * (((s.route.size()-2) * candidateList.size())- 1))];

        //Inserting the item into the solution and removing it from the candidate list
        s.route.insert(s.route.begin() + (nextNode->j) + 1, candidateList[nextNode->i]);
        s.cost += nextNode->delta;
        candidateList.erase(candidateList.begin() + nextNode->i);
    }
}

bool TSP::swap(){
    tMove bestSwap = {0, 0, INFINITY};  //Here we set the cost to INFINITY
    double delta, rmDelta;

    timer.setTime(1);
    //Repeating until the swap with lowest delta is found
    for(int i = 1; i < dimension - 2; i++){
        rmDelta = -matrix[s.route[i]][s.route[i-1]]
                  -matrix[s.route[i]][s.route[i+1]];
        for(int j = i + 1; j < dimension - 1; j++){
            if(j-i != 1)
                delta =  rmDelta
                        +matrix[s.route[i]][s.route[j-1]]
                        +matrix[s.route[i]][s.route[j+1]]
                        +matrix[s.route[j]][s.route[i-1]]
                        +matrix[s.route[j]][s.route[i+1]]
                        -matrix[s.route[j]][s.route[j-1]]
                        -matrix[s.route[j]][s.route[j+1]];
            else
                delta =  matrix[s.route[i]][s.route[j+1]]
                        +matrix[s.route[j]][s.route[i-1]]
                        -matrix[s.route[i]][s.route[i-1]]
                        -matrix[s.route[j]][s.route[j+1]];
                 
            
            if(delta < bestSwap.delta){
                bestSwap.i = i;
                bestSwap.j = j;
                bestSwap.delta = delta;
            }
        }
    }

    //Making the swap in the route and inserting the delta in the cost
    if(bestSwap.delta < 0){
        s.cost = s.cost + bestSwap.delta;
        std::swap(s.route[bestSwap.i], s.route[bestSwap.j]);
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
    for(int i = 1; i < dimension - 3; i++){
        for(int j = i + 2; j < dimension - 1; j++){
            delta =  matrix[s.route[i]][s.route[j+1]]
                    +matrix[s.route[j]][s.route[i-1]]
                    -matrix[s.route[i]][s.route[i-1]]
                    -matrix[s.route[j]][s.route[j+1]];
            
            if(delta < bestReversion.delta){
                bestReversion.i = i;
                bestReversion.j = j;
                bestReversion.delta = delta;
            }
        }
    } 

    if(bestReversion.delta < 0){
        s.cost = s.cost + bestReversion.delta;
        std::reverse(s.route.begin() + bestReversion.i, s.route.begin() + bestReversion.j+1);
        timer.setTime(2);
        return true;
    }

    timer.setTime(2);
    return false;
}

bool TSP::reinsert(int num){
    tMove bestReinsertion = {0, 0, INFINITY};
    double delta, rmDelta;

    timer.setTime(2+num);
    //These loops are searching for an j position to reinsert a subsequence that starts at i and goes to i+(num-1)
    for(int i = 1; i < dimension - num; i++){
        rmDelta =  matrix[s.route[i-1]][s.route[i+num]]
                  -matrix[s.route[i-1]][s.route[i]]
                  -matrix[s.route[i+(num-1)]][s.route[i+num]];
        for(int j = 0; j < dimension - (num+1); j++){   //j starts at 0 due to the fact that the subsequence starting from i will be inserted one position after it
            //Checking if the j index is the same as the beginning of the subsequence
            if(j+1 == i){
                j += num;   //If it really is, it will jump the whole subsequence
                continue;
            }
            delta =  rmDelta
                    +matrix[s.route[j]][s.route[i]]
                    +matrix[s.route[i+(num-1)]][s.route[j+1]]
                    -matrix[s.route[j]][s.route[j+1]];
            
            if(delta < bestReinsertion.delta){
                bestReinsertion.i = i;
                bestReinsertion.j = j;
                bestReinsertion.delta = delta;
            }
        }
    }
    
    if(bestReinsertion.delta < 0){
        s.cost = s.cost + bestReinsertion.delta;

        std::vector<int> subroute(s.route.begin() + bestReinsertion.i, s.route.begin() + bestReinsertion.i + num);
        s.route.erase(s.route.begin() + bestReinsertion.i, s.route.begin() + bestReinsertion.i + num);

        if(bestReinsertion.i > bestReinsertion.j)
            s.route.insert(s.route.begin() + (bestReinsertion.j+1), subroute.begin(), subroute.end());
        else
            s.route.insert(s.route.begin() + bestReinsertion.j-(num-1), subroute.begin(), subroute.end());  //If the subsequence is inserted in a j position that is ahead of the previous(i), it is needed to correct the index after the deletion
        
        timer.setTime(2+num);
        return true;
    }

    timer.setTime(2+num);
    return false;
}

void TSP::perturb(){
    int iSize = random(ceil(dimension/10.0)-1),    //min = 1 & max = dimension/10 - 1
        jSize = random(ceil(dimension/10.0)-1);

    int i = random(dimension - (iSize+jSize+2)),
        j = random(dimension - (i+iSize+jSize+1)) + (i+iSize);

    std::vector<int> subroute1(s.route.begin() + i, s.route.begin() + i + iSize + 1),
                     subroute2(s.route.begin() + j, s.route.begin() + j + jSize + 1);

    s.cost -=  matrix[s.route[i-1]][s.route[i]]
            +matrix[s.route[i+iSize]][s.route[i+iSize+1]]
            +((i+iSize+1 == j)? 0 : matrix[s.route[j-1]][s.route[j]])   //Checks if the subsequences are adjacent
            +matrix[s.route[j+jSize]][s.route[j+jSize+1]];

    s.route.erase(s.route.begin() + j, s.route.begin() + j + jSize + 1);
    s.route.erase(s.route.begin() + i, s.route.begin() + i + iSize + 1);
    s.route.insert(s.route.begin() + i, subroute2.begin(), subroute2.end());
    s.route.insert(s.route.begin() + j + (jSize - iSize), subroute1.begin(), subroute1.end());

    s.cost +=  matrix[s.route[i-1]][s.route[i]]
            +matrix[s.route[i+jSize]][s.route[i+jSize+1]]
            +((i+iSize+1 == j)? 0 : matrix[s.route[j + (jSize-iSize)-1]][s.route[j + (jSize-iSize)]])
            +matrix[s.route[j+jSize]][s.route[j+jSize+1]];
}



void TSP::printSolution(){
    for(int i = 0; i <= dimension; i++)
        printf("%d%s", final.route[i]+1, i == dimension?"\n":", ");
}

void TSP::printTimes(){
    std::cout << "Total time: " << timer.getTotalTime() << " (s)\n"
              << "| Construction execution time: " << timer.getConstructionTime() << " (s)\n"
              << "| Swap execution time: " << timer.getSwapTime() << " (s)\n"
              << "| 2-opt execution time: " << timer.getRevertTime() << " (s)\n"
              << "| Or-opt execution time: " << timer.getReinsertion1Time() << " (s)\n"
              << "| Or-opt2 execution time: " << timer.getReinsertion2Time() << " (s)\n"
              << "| Or-opt3 execution time: " << timer.getReinsertion3Time() << " (s)\n\n";
}

std::vector<double>* TSP::getTimes(){
    std::vector<double> *times = new std::vector<double>;
    times->resize(7);

    (*times)[0] = timer.getTotalTime();
    (*times)[1] = timer.getConstructionTime();
    (*times)[2] = timer.getSwapTime();
    (*times)[3] = timer.getRevertTime();
    (*times)[4] = timer.getReinsertion1Time();
    (*times)[5] = timer.getReinsertion2Time();
    (*times)[6] = timer.getReinsertion3Time();

    return times;
}

double TSP::getCost(){
    return final.cost;
}

double TSP::getRealCost(){
    double sum = 0;

    for(int i = 0; i < dimension; i++){
        sum+= matrix[final.route[i]][final.route[i+1]];
    }
    
    return sum;
}

void TSP::printMatrix(){
    printf("Dimension: %d\n\n", dimension);
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            char endian = ((j+1)==dimension) ? '\n' : ' ';
            std::cout <<  matrix[i][j] << endian;
        }
    }
}

//------------============ Debugging functions ============------------

void TSP::printCurrentSolution(bool returnBest){
    if(returnBest)
        for(int i = 0; i < best.route.size(); i++)
            printf("%d%s", best.route[i]+1, i+1 == best.route.size()?"\n":", ");
    else
        for(int i = 0; i < s.route.size(); i++)
            printf("%d%s", s.route[i]+1, i+1 == s.route.size()?"\n":", ");
}

double TSP::getCurrentRealCost(bool returnBest){
    double sum = 0;

    if(returnBest)
        for(int i = 0; i < dimension; i++)
            sum+= matrix[best.route[i]][best.route[i+1]];
    else
        for(int i = 0; i < dimension; i++)
            sum+= matrix[s.route[i]][s.route[i+1]];
    
    return sum;
}

//------------============================================------------