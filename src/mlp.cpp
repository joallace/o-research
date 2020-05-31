#include "mlp.h"

MLP::MLP(double ***mPointer, int dimension): Problem(mPointer, dimension){
    // Allocating the cost vectors
    s_.cost.resize(dimension_+1, std::vector<tCost>(dimension_+1));
    // best_.cost.resize(dimension_+1, std::vector<tCost>(dimension_+1));
    // final_.cost.resize(dimension_+1, std::vector<tCost>(dimension_+1));
    // final_.cost[0][final_.LAST].c = INFINITY;
    
    // Defining variables
    int i, max_iterations = std::min(100, dimension_);
    std::vector<char> neighbor_list;

    // GILS
    for(int i_max = 0; i_max < MLP_IMAX; i_max++){
        // Construction
        timer_.setTime(0);
        construction();
        timer_.setTime(0);

        // Setting up the depot
        s_.cost[0][0] = {0, 0, 0};
        s_.cost[s_.LAST][s_.LAST] = {0, 0, 0};

        // Computing the cost for each node 
        for(int i = 1; i < s_.route.size(); i++){
            s_.cost[i][i] = {1, 0, 0};
        }

        fillCost();

        if(i_max == 0)
            final_ = s_;
        best_ = s_;

        // RVND
        for(int i_ils = 0; i_ils < max_iterations; i_ils++){
            neighbor_list = DEFAULT_NEIGHBORLIST;
            while(!neighbor_list.empty()){
                i = random(neighbor_list.size())-1;
                switch(neighbor_list[i]){
                    case 1:
                        if(!swap())
                            neighbor_list.erase(neighbor_list.begin() + i);
                        else if(neighbor_list.size() != NEIGHBORLIST_SIZE)
                            neighbor_list = DEFAULT_NEIGHBORLIST;
                        break;

                    case 2:
                        if(!revert())
                            neighbor_list.erase(neighbor_list.begin() + i);
                        else if(neighbor_list.size() != NEIGHBORLIST_SIZE)
                            neighbor_list = DEFAULT_NEIGHBORLIST;
                        break;

                    case 3:
                        if(!reinsert(1))
                            neighbor_list.erase(neighbor_list.begin() + i);
                        else if(neighbor_list.size() != NEIGHBORLIST_SIZE)
                            neighbor_list = DEFAULT_NEIGHBORLIST;
                        break;

                    case 4:
                        if(!reinsert(2))
                            neighbor_list.erase(neighbor_list.begin() + i);
                        else if(neighbor_list.size() != NEIGHBORLIST_SIZE)
                            neighbor_list = DEFAULT_NEIGHBORLIST;
                        break;

                    case 5:
                        if(!reinsert(3))
                            neighbor_list.erase(neighbor_list.begin() + i);
                        else if(neighbor_list.size() != NEIGHBORLIST_SIZE)
                            neighbor_list = DEFAULT_NEIGHBORLIST;
                        break;
                }
            }
            if(s_.cost[0][s_.LAST].c < best_.cost[0][best_.LAST].c){
                best_ = s_;
                i_ils = 0;
            }
            else{
                s_ = best_;
            }

            perturb();
        }
        if(best_.cost[0][best_.LAST].c < final_.cost[0][final_.LAST].c){
            final_ = best_;
            std::cout << "New minimum: " << final_.cost[0][final_.LAST].c << "\n";
        }

        s_.route.clear();
    }

    timer_.stop();
}

// Concatenate two subsequences
inline void MLP::concatenate(tCost &s1, const tCost &s2, int s1_last, int s2_first) const{
    s1.c += s2.w*(s1.t + matrix_[s_.route[s1_last]][s_.route[s2_first]]) + s2.c;
    s1.t += matrix_[s_.route[s1_last]][s_.route[s2_first]] + s2.t;
    s1.w += s2.w;
}

// Constructs a feasible initial solution
void MLP::construction(){
    int last = 0,
        i = 0,
        interval;

    s_.route.push_back(0);

    // Filling up the candidate list
    for(int i = 1; i < dimension_; i++)
        candidate_list_.push_back(i);

    // Searching for the next node with lowest cost relative to the last node to insert in s_
    while(!candidate_list_.empty()){
        std::sort(candidate_list_.begin(), candidate_list_.end(), 
            [&](int j, int k) -> bool{
                return matrix_[last][j] < matrix_[last][k];
            }
        );

        interval = (int) (random(25)/100.0 * candidate_list_.size());
        last = random(interval == 0 ? 1 : interval) - 1;

        s_.route.push_back(candidate_list_[last]);
        candidate_list_.erase(candidate_list_.begin() + last);
    }
    
    s_.route.push_back(0);
}

// Fill the solution cost vector
void MLP::fillCost(){
    // Computing the cost for each subsequence
    for(int i = 0; i < s_.route.size(); i++){
        for(int j = i+1; j < s_.route.size(); j++){
            s_.cost[i][j].t = matrix_[s_.route[j-1]][s_.route[j]] 
                             +s_.cost[i][j-1].t;

            s_.cost[i][j].w = s_.cost[i][j-1].w
                             +s_.cost[j][j].w;

            s_.cost[i][j].c = s_.cost[i][j-1].c
                             +s_.cost[j][j].c
                             +s_.cost[j][j].w * (s_.cost[i][j-1].t + matrix_[s_.route[j-1]][s_.route[j]]);


            s_.cost[j][i].t = s_.cost[i][j].t;

            s_.cost[j][i].w = s_.cost[i][j].w;
            
            s_.cost[j][i].c = s_.cost[j-1][i].c
                             +s_.cost[j][j].c
                             +s_.cost[j][j].w * (s_.cost[j-1][i].t + matrix_[s_.route[j]][s_.route[j-1]]);
        }
    }
}

// Computes the cost in the [first, last] interval
void MLP::computeCost(int first, int last){
    for(int i = 0; i <= last; i++){
        for(int j = i<first? first : i+1; j < s_.route.size(); j++){
            s_.cost[i][j].t = matrix_[s_.route[j-1]][s_.route[j]] 
                             +s_.cost[i][j-1].t;

            s_.cost[i][j].w = s_.cost[i][j-1].w
                             +s_.cost[j][j].w;

            s_.cost[i][j].c = s_.cost[i][j-1].c
                             +s_.cost[j][j].c
                             +s_.cost[j][j].w * (s_.cost[i][j-1].t + matrix_[s_.route[j-1]][s_.route[j]]);


            s_.cost[j][i].t = s_.cost[i][j].t;

            s_.cost[j][i].w = s_.cost[i][j].w;
            
            s_.cost[j][i].c = s_.cost[j-1][i].c
                             +s_.cost[j][j].c
                             +s_.cost[j][j].w * (s_.cost[j-1][i].t + matrix_[s_.route[j]][s_.route[j-1]]);
        }
    }
}

// A function that searches for the best nodes i and j to swap 
bool MLP::swap(){ 
    tMove<tCost> best_swap = {0, 0, {0, 0, INFINITY}};  //Here we set the cost to INFINITY
    tCost cost;

    timer_.setTime(1);
    // Repeating until the swap with lowest cost is found
    for(int i = 1; i < s_.route.size() - 2; i++){
        for(int j = i + 2; j < s_.route.size() - 1; j++){
            cost = s_.cost[0][i-1];
            concatenate(cost, s_.cost[j][j], i-1, i);
            concatenate(cost, s_.cost[i+1][j-1], i, i+1);
            concatenate(cost, s_.cost[i][i], j-1, j);
            concatenate(cost, s_.cost[j+1][s_.LAST], j, j+1);
            
            if(cost.c < best_swap.cost.c){
                best_swap = {i, j, cost};
            }
        }
    }

    // Making the swap in the route and inserting the cost in the cost
    if(best_swap.cost.c < s_.cost[0][s_.LAST].c){
        std::swap(s_.route[best_swap.i], s_.route[best_swap.j]);
        computeCost(best_swap.i, best_swap.j);
        timer_.setTime(1);
        return true;
    }

    timer_.setTime(1);
    return false;
}

// A function that searches for the best range [i,j] to reverse
bool MLP::revert(){ 
    tMove<tCost> best_reversion = {0, 0, {0, 0, INFINITY}};
    tCost cost;

    timer_.setTime(2);
    for(int i = 1; i < s_.route.size() - 3; i++){
        for(int j = i + 1; j < s_.route.size() - 1; j++){
            cost = s_.cost[0][i-1];
            concatenate(cost, s_.cost[j][i], i-1, i);
            concatenate(cost, s_.cost[j+1][s_.LAST], j, j+1);
            
            if(cost.c < best_reversion.cost.c){
                best_reversion = {i, j, cost};
            }
        }
    } 

    if(best_reversion.cost.c < s_.cost[0][s_.LAST].c){
        std::reverse(s_.route.begin() + best_reversion.i, s_.route.begin() + best_reversion.j+1);
        computeCost(best_reversion.i, best_reversion.j);
        timer_.setTime(2);
        return true;
    }

    timer_.setTime(2);
    return false;
}

// A function that searches for the best j position to reinsert a subsequence [i,num)
bool MLP::reinsert(int num){ 
    tMove<tCost> best_reinsertion = {0, 0, {0, 0, INFINITY}};
    tCost cost;

    timer_.setTime(2+num);
    for(int i = 1; i < s_.route.size() - num; i++){
        for(int j = 1; j < s_.route.size() - num; j++){
            // Checking if the j index is the same as the beginning of the subsequence
            if(j != i){       
                if(j > i){
                    cost = s_.cost[0][i-1];
                    concatenate(cost, s_.cost[i+num][j], i-1, i+num);
                    concatenate(cost, s_.cost[i][i+(num-1)], j, i);
                    concatenate(cost, s_.cost[j+1][s_.LAST], i+(num-1), j+1);
                }else{
                    cost = s_.cost[0][j];
                    concatenate(cost, s_.cost[i][i+(num-1)], j, i);
                    concatenate(cost, s_.cost[j+1][i-1], i+(num-1), j+1);
                    concatenate(cost, s_.cost[i+num][s_.LAST], i-1, i+num);
                }
                
                if(cost.c < best_reinsertion.cost.c){
                    best_reinsertion = {i, j, cost};
                }
            }
        }
    }
    
    if(best_reinsertion.cost.c < s_.cost[0][s_.LAST].c){    
        if (best_reinsertion.i < best_reinsertion.j){
            std::rotate(s_.route.begin() + best_reinsertion.i, s_.route.begin() + best_reinsertion.i+num, s_.route.begin() + best_reinsertion.j+num);
            computeCost(best_reinsertion.i, best_reinsertion.j + num-1);
        }
        else{
            std::rotate(s_.route.begin() + best_reinsertion.j, s_.route.begin() + best_reinsertion.i, s_.route.begin() + best_reinsertion.i+num);
            computeCost(best_reinsertion.j, best_reinsertion.i + num-1);
        }

        timer_.setTime(2+num);
        return true;
    }

    timer_.setTime(2+num);
    return false;
}

// A function that perturbs the solution using the double-bridge method
void MLP::perturb(){
    int i_size = random(ceil(dimension_/10.0)-1),    //min = 1 & max = dimension_/10 - 1
        j_size = random(ceil(dimension_/10.0)-1);

    int i = random(dimension_ - (i_size+j_size+2)),
        j = random(dimension_ - (i+i_size+j_size+1)) + (i+i_size);

    std::vector<int> sub_route1(s_.route.begin() + i, s_.route.begin() + i + i_size + 1),
                     sub_route2(s_.route.begin() + j, s_.route.begin() + j + j_size + 1);

    s_.route.erase(s_.route.begin() + j, s_.route.begin() + j + j_size + 1);
    s_.route.erase(s_.route.begin() + i, s_.route.begin() + i + i_size + 1);
    s_.route.insert(s_.route.begin() + i, sub_route2.begin(), sub_route2.end());
    s_.route.insert(s_.route.begin() + j + (j_size - i_size), sub_route1.begin(), sub_route1.end());

    computeCost(i, j + j_size);
}

double MLP::getCost(){
    return final_.cost[0][final_.LAST].c;
}

double MLP::getRealCost(){
    double sum = 0;

    for(int i = 0; i <= dimension_; i++)
        for(int j = 0; j < i; j++)
            sum += matrix_[final_.route[j]][final_.route[j+1]];
    
    return sum;
}

double MLP::getSolutionCost(tSolution<std::vector<std::vector<tCost>>> &solution){
    double sum = 0;

    for(int i = 0; i <= dimension_; i++)
        for(int j = 0; j < i; j++)
            sum += matrix_[solution.route[j]][solution.route[j+1]];
    
    return sum;
}

void MLP::printSolution(){
    for(int i = 0; i <= dimension_; i++)
        printf("%d%s", final_.route[i]+1, i == dimension_?"\n":", ");
}

