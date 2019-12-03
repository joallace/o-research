#include "Timer.h"

Timer::Timer(){
    totalT0 = high_resolution_clock::now(); 

    construction =
    swap =
    revert =
    reinsertion1 =
    reinsertion2 =
    reinsertion3 = 0;
}

void Timer::setTime(char part){
    if(!hasRun[part])
        t0 = high_resolution_clock::now();
    else{
        t1 = high_resolution_clock::now();
        switch (part){
            case 0:
                construction += (double) duration_cast<nanoseconds>(t1 - t0).count();
                break;
            
            case 1:
                swap += (double) duration_cast<nanoseconds>(t1 - t0).count();
                break;

            case 2:
                revert += (double) duration_cast<nanoseconds>(t1 - t0).count();
                break;

            case 3:
                reinsertion1 += (double) duration_cast<nanoseconds>(t1 - t0).count();
                break;

            case 4:
                reinsertion2 += (double) duration_cast<nanoseconds>(t1 - t0).count();
                break;

            case 5:
                reinsertion3 += (double) duration_cast<nanoseconds>(t1 - t0).count();
                break;
        }
    }
    hasRun[part] = !hasRun[part];
}

void Timer::stop(){
    totalT1 = high_resolution_clock::now();
    total = (double) duration_cast<nanoseconds>(totalT1 - totalT0).count();
}

double Timer::getConstructionTime(){
    return construction/1000000000;
}

double Timer::getSwapTime(){
    return swap/1000000000;
}

double Timer::getRevertTime(){
    return revert/1000000000;
}

double Timer::getReinsertion1Time(){
    return reinsertion1/1000000000;
}

double Timer::getReinsertion2Time(){
    return reinsertion2/1000000000;
}

double Timer::getReinsertion3Time(){
    return reinsertion3/1000000000;
}

double Timer::getTotalTime(){
    return total/1000000000;
}