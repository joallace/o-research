#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using namespace std::chrono;

class Timer{
    high_resolution_clock::time_point t0, t1, totalT0, totalT1;  //Variables used to measure the time delta

    int64_t durations[7] = {};

    bool hasRun;

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

        int64_t* getPointer();
};


#endif // TIMER_H