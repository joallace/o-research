#include "include/tsp.h"

TSP::TSP(double ***matrix_pointer, int dimension, int iterations): MetaheuristicProblem(matrix_pointer, dimension){
    final_.cost = INFINITY;
    
    // Defining variables
    int i, max_iterations = dimension_>=150 ? dimension_/2 : dimension_;
    std::vector<char> neighbor_list;

    // GILS
    for(int i_max = 0; i_max < iterations; i_max++){
        s_.cost = 0;

        // Filling up the candidate list
        for(i = 0; i < dimension_; i++)
            candidate_list_.push_back(i);

        // Construction
        timer_.setTime(0);
        
        subtour(); // Creating a initial subtour
        initialRoute(); // Filling up the solution vector feasibly

        timer_.setTime(0);

        best_.cost = INFINITY;

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
            if(s_.cost < best_.cost){
                best_ = s_;
                i_ils = 0;
            }
            else
                s_ = best_;

            perturb();
        }
        if(best_.cost < final_.cost){
            final_ = best_;
            std::cout << "New minimum: " << final_.cost << "\n";
        }

        s_.route.clear();
    }
    
    timer_.stop();
}

void TSP::subtour(){
    //Obtaining an initial item randomly
    int first = random(dimension_-1);

    //Inserting it into the solution and removing it from the candidate list
    s_.route.push_back(first);
    candidate_list_.erase(candidate_list_.begin() + first);

    //Inserting random items from the candidate list into the solution
    for(int i = 0; i < SUBTOUR_SIZE; i++){
        int j = random(candidate_list_.size()) - 1;
        s_.cost += matrix_[s_.route[i]][candidate_list_[j]];
        s_.route.push_back(candidate_list_[j]);
        candidate_list_.erase(candidate_list_.begin() + j);  
    }

    //Finishing the Hamiltonian cycle
    s_.route.push_back(first);
    s_.cost += matrix_[s_.route[SUBTOUR_SIZE]][s_.route[SUBTOUR_SIZE+1]];
}

void TSP::initialRoute(){    
    //Repeating until a feasible initial solution is found
    while (!candidate_list_.empty()){
        //Calculating the insertion cost of each one of the remaining candidates
        tMove<double> cost_vector[(s_.route.size()-2) * candidate_list_.size()];

        for(int i = 1, k = 0; i < s_.route.size()-1; i++){
            for(int j = 0; j < candidate_list_.size(); j++){
                //Assigning the indexes to the node members
                cost_vector[k].i = j;
                cost_vector[k].j = i;

                //Calculating the insertion cost and inserting the node into the vector
                cost_vector[k++].cost =  matrix_[s_.route[i]][candidate_list_[j]] 
                                        +matrix_[candidate_list_[j]][s_.route[i+1]]
                                        -matrix_[s_.route[i]][s_.route[i+1]];
            }
        }
    
        //Sorting the cost_vector
        std::sort(&cost_vector[0], &cost_vector[(s_.route.size()-2) * candidate_list_.size()]);
        
        //Obtaining an item in a random interval of the cost_vector
        tMove<double>* next_node = &cost_vector[random((int)random(10)/10.0 * (((s_.route.size()-2) * candidate_list_.size())- 1))];

        //Inserting the item into the solution and removing it from the candidate list
        s_.route.insert(s_.route.begin() + (next_node->j) + 1, candidate_list_[next_node->i]);
        s_.cost += next_node->cost;
        candidate_list_.erase(candidate_list_.begin() + next_node->i);
    }
}

// A function that searches for the best nodes i and j to swap 
bool TSP::swap(){ 
    tMove<double> best_swap = {0, 0, INFINITY};  //Here we set the cost to INFINITY
    double delta, rm_delta;

    timer_.setTime(1);
    // Repeating until the swap with lowest delta is found
    for(int i = 1; i < s_.route.size() - 2; i++){
        rm_delta = -matrix_[s_.route[i]][s_.route[i-1]]
                   -matrix_[s_.route[i]][s_.route[i+1]];
        for(int j = i + 2; j < s_.route.size() - 1; j++){
            delta =  rm_delta
                    +matrix_[s_.route[i]][s_.route[j-1]]
                    +matrix_[s_.route[i]][s_.route[j+1]]
                    +matrix_[s_.route[j]][s_.route[i-1]]
                    +matrix_[s_.route[j]][s_.route[i+1]]
                    -matrix_[s_.route[j]][s_.route[j-1]]
                    -matrix_[s_.route[j]][s_.route[j+1]];
            
            if(delta < 0 && delta < best_swap.cost)
                best_swap = {i, j, delta};
        }
    }

    // Making the swap in the route and inserting the delta in the cost
    if(best_swap.cost < 0){
        s_.cost = s_.cost + best_swap.cost;
        std::swap(s_.route[best_swap.i], s_.route[best_swap.j]);
        timer_.setTime(1);
        return true;
    }

    timer_.setTime(1);
    return false;
}

// A function that searches for the best range [i,j] to reverse
bool TSP::revert(){ 
    tMove<double> best_reversion = {0, 0, INFINITY};
    double delta;

    timer_.setTime(2);
    for(int i = 1; i < s_.route.size() - 3; i++){
        for(int j = i + 1; j < s_.route.size() - 1; j++){
            delta =  matrix_[s_.route[i]][s_.route[j+1]]
                    +matrix_[s_.route[j]][s_.route[i-1]]
                    -matrix_[s_.route[i]][s_.route[i-1]]
                    -matrix_[s_.route[j]][s_.route[j+1]];
            
            if(delta < 0 && delta < best_reversion.cost)
                best_reversion = {i, j, delta};
        }
    } 

    if(best_reversion.cost < 0){
        s_.cost = s_.cost + best_reversion.cost;
        std::reverse(s_.route.begin() + best_reversion.i, s_.route.begin() + best_reversion.j+1);
        timer_.setTime(2);
        return true;
    }

    timer_.setTime(2);
    return false;
}

// A function that searches for the best j position to reinsert a subsequence [i,num)
bool TSP::reinsert(int num){ 
    tMove<double> best_reinsertion = {0, 0, INFINITY};
    double delta, rm_delta;

    timer_.setTime(2+num);
    for(int i = 1; i < s_.route.size() - num; i++){
        rm_delta = matrix_[s_.route[i-1]][s_.route[i+num]]
                  -matrix_[s_.route[i-1]][s_.route[i]]
                  -matrix_[s_.route[i+(num-1)]][s_.route[i+num]];
        for(int j = 1; j < s_.route.size() - num; j++){
            // Checking if the j index is the same as the beginning of the subsequence
            if(j != i){       
                if(j > i)
                    delta =  rm_delta
                            +matrix_[s_.route[j+(num-1)]][s_.route[i]]
                            +matrix_[s_.route[i+(num-1)]][s_.route[j+num]]
                            -matrix_[s_.route[j+(num-1)]][s_.route[j+num]];
                else
                    delta =  rm_delta 
                            +matrix_[s_.route[j-1]][s_.route[i]]
                            +matrix_[s_.route[i+(num-1)]][s_.route[j]] 
                            -matrix_[s_.route[j]][s_.route[j-1]];
                
                if(delta < 0 && delta < best_reinsertion.cost)
                    best_reinsertion = {i, j, delta};
            }
        }
    }
    
    if(best_reinsertion.cost < 0){
        s_.cost = s_.cost + best_reinsertion.cost;
        
        if (best_reinsertion.i < best_reinsertion.j)
            std::rotate(s_.route.begin() + best_reinsertion.i, s_.route.begin() + best_reinsertion.i+num, s_.route.begin() + best_reinsertion.j+num);
        else
            std::rotate(s_.route.begin() + best_reinsertion.j, s_.route.begin() + best_reinsertion.i, s_.route.begin() + best_reinsertion.i+num);

        timer_.setTime(2+num);
        return true;
    }

    timer_.setTime(2+num);
    return false;
}

// A function that perturbs the solution using the Double-bridge method
void TSP::perturb(){
    int i_size = random(ceil(dimension_/10.0)-1),    //min = 1 & max = dimension_/10 - 1
        j_size = random(ceil(dimension_/10.0)-1);

    int i = random(dimension_ - (i_size+j_size+2)),
        j = random(dimension_ - (i+i_size+j_size+1)) + (i+i_size);

    std::vector<int> sub_route1(s_.route.begin() + i, s_.route.begin() + i + i_size + 1),
                     sub_route2(s_.route.begin() + j, s_.route.begin() + j + j_size + 1);

    s_.cost -=  matrix_[s_.route[i-1]][s_.route[i]]
               +matrix_[s_.route[i+i_size]][s_.route[i+i_size+1]]
               +((i+i_size+1 == j)? 0 : matrix_[s_.route[j-1]][s_.route[j]])   //Checks if the subsequences are adjacent
               +matrix_[s_.route[j+j_size]][s_.route[j+j_size+1]];

    s_.route.erase(s_.route.begin() + j, s_.route.begin() + j + j_size + 1);
    s_.route.erase(s_.route.begin() + i, s_.route.begin() + i + i_size + 1);
    s_.route.insert(s_.route.begin() + i, sub_route2.begin(), sub_route2.end());
    s_.route.insert(s_.route.begin() + j + (j_size - i_size), sub_route1.begin(), sub_route1.end());

    s_.cost +=  matrix_[s_.route[i-1]][s_.route[i]]
               +matrix_[s_.route[i+j_size]][s_.route[i+j_size+1]]
               +((i+i_size+1 == j)? 0 : matrix_[s_.route[j + (j_size-i_size)-1]][s_.route[j + (j_size-i_size)]])
               +matrix_[s_.route[j+j_size]][s_.route[j+j_size+1]];
}

tSolution<double> TSP::getSolution(){
    return final_;
}

// Returns the final cost
double TSP::getCost(){
    return final_.cost;
}

double TSP::getRealCost(){
    double sum = 0;

    for(int i = 0; i < dimension_; i++)
        sum+= matrix_[final_.route[i]][final_.route[i+1]];
    
    return sum;
}

double TSP::getSolutionCost(tSolution<double> &solution){
    double sum = 0;

    for(int i = 0; i < dimension_; i++)
        sum+= matrix_[solution.route[i]][solution.route[i+1]];
    
    return sum;
}

void TSP::printSolution(){
    printRoute(final_.route);
}