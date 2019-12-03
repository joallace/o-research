#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using namespace std::chrono;

class Timer{
    high_resolution_clock::time_point t0, t1, totalT0, totalT1;  //Variables used to measure the time delta
    bool hasRun[6] = {};
    double construction,
           swap,
           revert,
           reinsertion1,
           reinsertion2,
           reinsertion3,
           total;

    public:
        Timer();
        
        void setTime(char part),
             setTotalTime(),
             stop();

        double getConstructionTime(),
               getSwapTime(),
               getRevertTime(),
               getReinsertion1Time(),
               getReinsertion2Time(),
               getReinsertion3Time(),
               getTotalTime();
};


#endif // TIMER_H