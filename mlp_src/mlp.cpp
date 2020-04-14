#include "mlp.h"

MLP::MLP(double ***mPointer, int dimension){
    // Setting up
    timer = Timer();
    matrix = *mPointer;
    this->dimension = dimension;
    s.cost.resize(dimension+1, std::vector<tCost>(dimension+1));
    
    // Defining variables
    int i, i_ILS = std::min(100, dimension);
    std::vector<char> neighborList;

    // GILS
    for(int iMax = 0; iMax < IMAX; iMax++){
        //---=== Construction ===---
        timer.setTime(0);
        construction();
        timer.setTime(0);
        //---====================---

        // Setting up the depot
        s.cost[0][0].t =
        s.cost[0][0].c =
        s.cost[0][0].w = 0;

        s.cost[s.LAST][s.LAST].t =
        s.cost[s.LAST][s.LAST].c =
        s.cost[s.LAST][s.LAST].w = 0;

        // Computing the cost for each node 
        for(int i = 1; i < s.route.size(); i++){
            s.cost[i][i].t =
            s.cost[i][i].c = 0;
            s.cost[i][i].w = 1;
        }

        fillCost();

        if(iMax == 0)
            final = s;
        best = s;

        // RVND
        for(int iILS = 0; iILS < i_ILS; iILS++){
            neighborList = DEFAULT_NEIGHBORLIST;
            while(!neighborList.empty()){
                i = random(neighborList.size())-1;
                switch(neighborList[i]){
                    case 1:
                        if(!swap())
                            neighborList.erase(neighborList.begin() + i);
                        else if(neighborList.size() != NEIGHBORLIST_SIZE)
                            neighborList = DEFAULT_NEIGHBORLIST;
                        break;

                    case 2:
                        if(!revert())
                            neighborList.erase(neighborList.begin() + i);
                        else if(neighborList.size() != NEIGHBORLIST_SIZE)
                            neighborList = DEFAULT_NEIGHBORLIST;
                        break;

                    case 3:
                        if(!reinsert(1))
                            neighborList.erase(neighborList.begin() + i);
                        else if(neighborList.size() != NEIGHBORLIST_SIZE)
                            neighborList = DEFAULT_NEIGHBORLIST;
                        break;

                    case 4:
                        if(!reinsert(2))
                            neighborList.erase(neighborList.begin() + i);
                        else if(neighborList.size() != NEIGHBORLIST_SIZE)
                            neighborList = DEFAULT_NEIGHBORLIST;
                        break;

                    case 5:
                        if(!reinsert(3))
                            neighborList.erase(neighborList.begin() + i);
                        else if(neighborList.size() != NEIGHBORLIST_SIZE)
                            neighborList = DEFAULT_NEIGHBORLIST;
                        break;
                }
            }
            if(s.cost[0][s.LAST].c < best.cost[0][best.LAST].c){
                best = s;
                iILS = 0;
            }
            else{
                s = best;
            }
            perturb();
            fillCost();
        }
        if(best.cost[0][best.LAST].c < final.cost[0][final.LAST].c){
            std::cout << "Previous: " << final.cost[0][final.LAST].c << "(" << getSolutionCost(final) << ")\n";
            final = best;
            std::cout << "New FINAL cost: " << final.cost[0][final.LAST].c << "(" << getSolutionCost(final) << ")\n\n";
        }
        s.route.clear();
    }
    timer.stop();
}

// Just a function that returns a random number from [1, num]
inline int MLP::random(int num) const{
    return (rand()%num)+1;
}

// Concatenate two subsequences
inline void MLP::concatenate(tCost &s1, const tCost &s2, int s1_last, int s2_first) const{
    s1.c += s2.w*(s1.t + matrix[s.route[s1_last]][s.route[s2_first]]) + s2.c;
    s1.t += matrix[s.route[s1_last]][s.route[s2_first]] + s2.t;
    s1.w += s2.w;
}

// Constructs a feasible initial solution
void MLP::construction(){
    int last = 0,
        i = 0,
        interval;

    s.route.push_back(0);

    // Filling up the candidate list
    for(int i = 1; i < dimension; i++)
        candidateList.push_back(i);

    // Searching for the next node with lowest cost relative to the last node to insert in s
    while(!candidateList.empty()){
        std::sort(candidateList.begin(), candidateList.end(), 
            [&](int j, int k) -> bool{
                return matrix[last][j] < matrix[last][k];
            }
        );

        interval = (int) (random(25)/100.0 * candidateList.size());
        last = random(interval == 0 ? 1 : interval) - 1;

        s.route.push_back(candidateList[last]);
        candidateList.erase(candidateList.begin() + last);
    }
    s.route.push_back(0);
}

// Fill the solution cost vector
void MLP::fillCost(){
    // Computing the cost for each subsequence
    for(int i = 0; i < s.route.size(); i++){
        for(int j = i+1; j < s.route.size(); j++){
            s.cost[i][j].t =  matrix[s.route[j-1]][s.route[j]] 
                             +s.cost[i][j-1].t;

            s.cost[i][j].w =  s.cost[i][j-1].w
                             +s.cost[j][j].w;

            s.cost[i][j].c =  s.cost[i][j-1].c
                             +s.cost[j][j].c
                             +s.cost[j][j].w * (s.cost[i][j-1].t + matrix[s.route[j-1]][s.route[j]]);


            s.cost[j][i].t = s.cost[i][j].t;

            s.cost[j][i].w = s.cost[i][j].w;
            
            s.cost[j][i].c =  s.cost[j-1][i].c
                             +s.cost[j][j].c
                             +s.cost[j][j].w * (s.cost[j-1][i].t + matrix[s.route[j]][s.route[j-1]]);
        }
    }
}

// Computes the cost in the [first, last] interval
void MLP::computeCost(int first, int last){
    for(int i = 0; i <= last; i++){
        for(int j = i<first? first : i+1; j < s.route.size(); j++){
            s.cost[i][j].t =  matrix[s.route[j-1]][s.route[j]] 
                             +s.cost[i][j-1].t;

            s.cost[i][j].w =  s.cost[i][j-1].w
                             +s.cost[j][j].w;

            s.cost[i][j].c =  s.cost[i][j-1].c
                             +s.cost[j][j].c
                             +s.cost[j][j].w * (s.cost[i][j-1].t + matrix[s.route[j-1]][s.route[j]]);


            s.cost[j][i].t = s.cost[i][j].t;

            s.cost[j][i].w = s.cost[i][j].w;
            
            s.cost[j][i].c =  s.cost[j-1][i].c
                             +s.cost[j][j].c
                             +s.cost[j][j].w * (s.cost[j-1][i].t + matrix[s.route[j]][s.route[j-1]]);
        }
    }
}

// A function that searches for the best nodes i and j to swap 
bool MLP::swap(){ 
    tMove bestSwap = {0, 0, {0, 0, INFINITY}};  //Here we set the cost to INFINITY
    tCost cost;

    timer.setTime(1);
    // Repeating until the swap with lowest cost is found
    for(int i = 1; i < s.route.size() - 2; i++){
        for(int j = i + 2; j < s.route.size() - 1; j++){
            cost = s.cost[0][i-1];
            concatenate(cost, s.cost[j][j], i-1, i);
            concatenate(cost, s.cost[i+1][j-1], i, i+1);
            concatenate(cost, s.cost[i][i], j-1, j);
            concatenate(cost, s.cost[j+1][s.LAST], j, j+1);
            
            if(cost.c < bestSwap.cost.c){
                bestSwap.i = i;
                bestSwap.j = j;
                bestSwap.cost = cost;
            }
        }
    }

    // Making the swap in the route and inserting the cost in the cost
    if(bestSwap.cost.c < s.cost[0][s.LAST].c){
        std::swap(s.route[bestSwap.i], s.route[bestSwap.j]);
        computeCost(bestSwap.i, bestSwap.j);
        timer.setTime(1);
        return true;
    }

    timer.setTime(1);
    return false;
}

// A function that searches for the best range [i,j] to reverse
bool MLP::revert(){ 
    tMove bestReversion = {0, 0, {0, 0, INFINITY}};
    tCost cost;

    timer.setTime(2);
    for(int i = 1; i < s.route.size() - 3; i++){
        for(int j = i + 1; j < s.route.size() - 1; j++){
            cost = s.cost[0][i-1];
            concatenate(cost, s.cost[j][i], i-1, i);
            concatenate(cost, s.cost[j+1][s.LAST], j, j+1);
            
            if(cost.c < bestReversion.cost.c){
                bestReversion.i = i;
                bestReversion.j = j;
                bestReversion.cost = cost;
            }
        }
    } 

    if(bestReversion.cost.c < s.cost[0][s.LAST].c){
        std::reverse(s.route.begin() + bestReversion.i, s.route.begin() + bestReversion.j+1);
        computeCost(bestReversion.i, bestReversion.j);
        timer.setTime(2);
        return true;
    }

    timer.setTime(2);
    return false;
}

// A function that searches for the best j position to reinsert a subsequence [i,num)
bool MLP::reinsert(int num){ 
    tMove bestReinsertion = {0, 0, {0, 0, INFINITY}};
    tCost cost;

    timer.setTime(2+num);
    for(int i = 1; i < s.route.size() - num; i++){
        for(int j = 1; j < s.route.size() - num; j++){
            // Checking if the j index is the same as the beginning of the subsequence
            if(j != i){       
                if(j > i){
                    cost = s.cost[0][i-1];
                    concatenate(cost, s.cost[i+num][j], i-1, i+num);
                    concatenate(cost, s.cost[i][i+(num-1)], j, i);
                    concatenate(cost, s.cost[j+1][s.LAST], i+(num-1), j+1);
                }else{
                    cost = s.cost[0][j];
                    concatenate(cost, s.cost[i][i+(num-1)], j, i);
                    concatenate(cost, s.cost[j+1][i-1], i+(num-1), j+1);
                    concatenate(cost, s.cost[i+num][s.LAST], i-1, i+num);
                }
                
                if(cost.c < bestReinsertion.cost.c){
                    bestReinsertion.i = i;
                    bestReinsertion.j = j;
                    bestReinsertion.cost = cost;
                }
            }
        }
    }
    
    if(bestReinsertion.cost.c < s.cost[0][s.LAST].c){    
        if (bestReinsertion.i < bestReinsertion.j){
            std::rotate(s.route.begin() + bestReinsertion.i, s.route.begin() + bestReinsertion.i+num, s.route.begin() + bestReinsertion.j+num);
            computeCost(bestReinsertion.i, bestReinsertion.j + num-1);
        }
        else{
            std::rotate(s.route.begin() + bestReinsertion.j, s.route.begin() + bestReinsertion.i, s.route.begin() + bestReinsertion.i+num);
            computeCost(bestReinsertion.j, bestReinsertion.i + num-1);
        }

        timer.setTime(2+num);
        return true;
    }

    timer.setTime(2+num);
    return false;
}

// A function that perturbs the solution using the Double-bridge method
void MLP::perturb(){
    int iSize = random(ceil(dimension/10.0)-1),    //min = 1 & max = dimension/10 - 1
        jSize = random(ceil(dimension/10.0)-1);

    int i = random(dimension - (iSize+jSize+2)),
        j = random(dimension - (i+iSize+jSize+1)) + (i+iSize);

    std::vector<int> subroute1(s.route.begin() + i, s.route.begin() + i + iSize + 1),
                     subroute2(s.route.begin() + j, s.route.begin() + j + jSize + 1);

    s.route.erase(s.route.begin() + j, s.route.begin() + j + jSize + 1);
    s.route.erase(s.route.begin() + i, s.route.begin() + i + iSize + 1);
    s.route.insert(s.route.begin() + i, subroute2.begin(), subroute2.end());
    s.route.insert(s.route.begin() + j + (jSize - iSize), subroute1.begin(), subroute1.end());
}

void MLP::printSolution(){
    for(int i = 0; i <= dimension; i++)
        printf("%d%s", final.route[i]+1, i == dimension?"\n":", ");
}

void MLP::printTimes(){
    std::cout << "Total time: " << timer.getTotalTime() << " (s)\n"
              << "| Construction execution time: " << timer.getConstructionTime() << " (s)\n"
              << "| Swap execution time: " << timer.getSwapTime() << " (s)\n"
              << "| 2-opt execution time: " << timer.getRevertTime() << " (s)\n"
              << "| Or-opt execution time: " << timer.getReinsertion1Time() << " (s)\n"
              << "| Or-opt2 execution time: " << timer.getReinsertion2Time() << " (s)\n"
              << "| Or-opt3 execution time: " << timer.getReinsertion3Time() << " (s)\n\n";
}

// Return the execution times in a vector
std::vector<double>* MLP::getTimes(){
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

double MLP::getCost(){
    return final.cost[0][final.route.size()-1].c;
}

double MLP::getRealCost(){
    double sum = 0;

    for(int i = 0; i <= dimension; i++)
        for(int j = 0; j < i; j++)
            sum += matrix[final.route[j]][final.route[j+1]];
    
    return sum;
}

void MLP::printMatrix(){
    printf("Dimension: %d\n\n", dimension);
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            char endian = ((j+1)==dimension) ? '\n' : ' ';
            std::cout <<  matrix[i][j] << endian;
        }
    }
}

//------------============ Debugging functions ============------------

void MLP::printSolution(tSolution &solution){
    for(int i = 0; i < solution.route.size(); i++)
        printf("%d%s", solution.route[i]+1, i+1 == solution.route.size()?"\n":", ");
}

void MLP::printRoute(std::vector<int> &route){
    for(int i = 0; i < route.size(); i++)
        printf("%d%s", route[i]+1, i+1 == route.size()?"\n":", ");
}

double MLP::getSolutionCost(tSolution &solution){
    double sum = 0;

    for(int i = 0; i <= dimension; i++)
        for(int j = 0; j < i; j++)
            sum += matrix[solution.route[j]][solution.route[j+1]];
    
    return sum;
}

//------------============================================------------