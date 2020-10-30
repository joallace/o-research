#ifndef MH_PROBLEM_H
#define MH_PROBLEM_H

#include "problem.h"

#define SUBTOUR_SIZE 3
#define NEIGHBORLIST_SIZE 5
#define DEFAULT_NEIGHBORLIST {1, 2, 3, 4, 5}

class MetaheuristicProblem : public Problem{
    protected:
        std::vector<int> candidate_list_;

        virtual void perturb() = 0;

        virtual bool swap() = 0,
                     revert() = 0,
                     reinsert(int num) = 0;

        int random(int num) const;
        
    public:
        MetaheuristicProblem(double ***matrix_pointer, int dimension);

        void printTimes();

        virtual double getRealCost() = 0;
};

#endif // MH_PROBLEM_H
