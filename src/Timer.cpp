#include "Timer.h"

Timer::Timer(){
    totalT0 = high_resolution_clock::now(); 

    construction =
    swap =
    revert =
    reinsertion1 =
    reinsertion2 =
    reinsertion3 = 0;

    hasRun.reset();
}

void Timer::setTime(char part){
    if(!hasRun[part])
        t0 = high_resolution_clock::now();
    else{
        t1 = high_resolution_clock::now();
        switch (part){
            case 0:
                construction += (double) duration_cast<seconds>(t1 - t0).count();
                break;
            
            case 1:
                swap += (double) duration_cast<seconds>(t1 - t0).count();
                break;

            case 2:
                revert += (double) duration_cast<seconds>(t1 - t0).count();
                break;

            case 3:
                reinsertion1 += (double) duration_cast<seconds>(t1 - t0).count();
                break;

            case 4:
                reinsertion2 += (double) duration_cast<seconds>(t1 - t0).count();
                break;

            case 5:
                reinsertion3 += (double) duration_cast<seconds>(t1 - t0).count();
                break;
        }
    }
    hasRun.flip(part);
}

void Timer::stop(){
    totalT1 = high_resolution_clock::now();
    total = (double) duration_cast<seconds>(totalT1 - totalT0).count();
}

double Timer::getConstructionTime(){
    return construction;
}

double Timer::getSwapTime(){
    return swap;
}

double Timer::getRevertTime(){
    return revert;
}

double Timer::getReinsertion1Time(){
    return reinsertion1;
}

double Timer::getReinsertion2Time(){
    return reinsertion2;
}

double Timer::getReinsertion3Time(){
    return reinsertion3;
}

double Timer::getTotalTime(){
    return total;
}