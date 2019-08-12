#include "TSP.h"

TSP::TSP(double ***mPointer, int dimension){
    matrix = *mPointer;
    this->dimension = dimension;
    cost = 0;

    //Preenchendo a lista de candidatos
    for(int i = 1; i <= dimension; i++){
        candidateList.push_back(i);
    }
    
    //Criando um subtour inicial
    subtour();

    //Preenchendo o vetor solucao viavelmente
    initialRoute();
    
    //swap();
    //revert();
    reinsert(3);
}

int TSP::random(int num){
    static bool seedGenerated = false;

    if(!seedGenerated){
        srand(time(NULL));
        seedGenerated = true;
    }

    return (rand()%num)+1;
}

void TSP::subtour(){
    //Obtendo um item incial aleatorio
    int first = random(dimension);

    //Inserindo-o na solucao e removendo-o da lista de candidatos
    route.push_back(first);
    candidateList.erase(candidateList.begin() + first-1);

    //Inserindo itens aleatorios da lista de candidatos na solucao
    for(int i = 0; i < SUBTOUR_SIZE; i++){
        int j = random(candidateList.size()) - 1;
        cost += matrix[route[i]][candidateList[j]];
        route.push_back(candidateList[j]);
        candidateList.erase(candidateList.begin() + j);  
    }

    //Terminando o ciclo hamiltoniano
    route.push_back(first);
    cost += matrix[route[SUBTOUR_SIZE]][route[SUBTOUR_SIZE+1]];
}

void TSP::initialRoute(){
    tMove* nextNode;
    
    //Repetindo ate obter uma solucao inicial viavel
    while (!candidateList.empty()){
        //Calculando o custo de insercao de cada um dos candidatos restantes
        std::vector <tMove*> costVector((route.size()-2) * candidateList.size());

        for(int i = 1, k = 0; i < route.size()-1; i++){
            for(int j = 0; j < candidateList.size(); j++){
                //Alocando um no de custo
                nextNode = (tMove*)malloc(sizeof(tMove));

                //Atribuindo os indices aos seus elementos
                nextNode->i = j;
                nextNode->j = i;

                //Calculando o custo de insercao e o inserindo no vetor
                nextNode->delta =  matrix[route[i]][candidateList[j]] 
                                  +matrix[candidateList[j]][route[i+1]]
                                  -matrix[route[i]][route[i+1]];

                costVector[k++] = nextNode;
            }
        }
    
        //Organizando o vetor de custo
        std::sort(costVector.begin(), costVector.end());
        
        //Obtendo um item em um intervalo aleatorio do vetor de custo
        nextNode = costVector[random((int)random(10)/10.0 * (costVector.size()- 1))];

        //Inserindo o item na solucao, e o removendo do vetor de candidatos
        route.insert(route.begin() + (nextNode->j) + 1, candidateList[nextNode->i]);
        cost += nextNode->delta;
        candidateList.erase(candidateList.begin() + nextNode->i);
    }
}

void TSP::swap(){
    //Criando um movimento de swap com delta infito, a fim de nao pegar lixo de memoria, e nao bugar o if
    tMove bestSwap = {0, 0, INFINITY};
    double delta;

    //Repetindo ate encontrar o swap com menor delta
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

    //Realizando o swap na rota, e inserindo o delta no custo
    if(bestSwap.delta < 0){
        cost = cost + bestSwap.delta;
        std::swap(route[bestSwap.i], route[bestSwap.j]);
    }
}

void TSP::revert(){
    tMove bestReversion = {0, 0, INFINITY};
    double delta;

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
    }
}

void TSP::reinsert(int num){
    tMove bestReinsertion = {0, 0, INFINITY};
    double delta;

    printSolution();

    for(int i = 1; i < dimension - (num-1); i++){
        for(int j = 0; j < dimension - num; j++){
            //Checando se ele esta dentro do intervalo que sera movimentado
            if(j+1 == i){
                j += num; //num-1 pq tem o j++ do for
                continue;
            }

            delta =  matrix[route[j]][route[i]]
                    +matrix[route[i+(num-1)]][route[j+1]]
                    +matrix[route[i-1]][route[route[i+num]]]
                    -matrix[route[i-1]][route[i]]
                    -matrix[route[i+(num-1)]][route[i+num]]
                    -matrix[route[j]][route[j+1]];

            std::cout << "INSERTING " << route[i] << " -> " << route[i+(num-1)] << " AFTER " << route[j]
                      << "\nCost:  " << route[j] << " -> " << route[i] << " : " << matrix[route[j]][route[i]] << "\n"
                      << "     + " << route[i+(num-1)] << " -> " << route[j+1] << " : " << matrix[route[i+(num-1)]][route[j+1]] << "\n"
                      << "     + " << route[i-1] << " -> " << route[i+num] << " : " << matrix[route[i-1]][route[route[i+num]]] << "\n"
                      << "     - " << route[i-1] << " -> " << route[i] << " : " << matrix[route[i-1]][route[i]] << "\n"
                      << "     - " << route[i+(num-1)] << " -> " << route[i+num] << " : " << matrix[route[i+(num-1)]][route[i+num]] << "\n"
                      << "     - " << route[j] << " -> " << route[j+1] << " : " << matrix[route[j]][route[j+1]] << "\n"
                      << "Total: " << delta << "\n\n";
                      
            
            if(bestReinsertion.delta > delta){
                bestReinsertion.i = i;
                bestReinsertion.j = j;
                bestReinsertion.delta = delta;
            }
        }
    }

    printf("REINSERT %d\n", num);
    printSolution();
    printf("Nodes: (%d-%d) -> %d\nCost: %f + (%f)\n", route[bestReinsertion.i], route[bestReinsertion.i+(num-1)], route[bestReinsertion.j], cost, bestReinsertion.delta);
    
    if(bestReinsertion.delta < 0){
        cost = cost + bestReinsertion.delta;

        std::vector<int> subroute(route.begin() + bestReinsertion.i, route.begin() + bestReinsertion.i + num);
        route.erase(route.begin() + bestReinsertion.i, route.begin() + bestReinsertion.i + num);
        route.insert(route.begin() + (bestReinsertion.j+1), subroute.begin(), subroute.end());
    }
    printSolution();
    printf("\n");
}

void TSP::printSolution(){
    for(int i = 0; i < route.size(); i++)
        printf("%d%s", route[i], i+1 == route.size()?"\n":", ");
}

double TSP::getCost(){
    return cost;
}

double TSP::getRealCost(){
    double sum = 0;

    for(int i = 0; i < dimension; i++){
        sum+= matrix[route[i]][route[i+1]];
    }
    
    return sum;
}

void TSP::printMatrix(){
    printf("Dimension: %d\n", dimension);
    for(int i = 1; i <= dimension; i++){
        for(int j = 1; j <= dimension; j++){
            char endian = ((j+1)>dimension) ? '\n' : ' ';
            std::cout <<  matrix[i][j] << endian;
        }
    }
}