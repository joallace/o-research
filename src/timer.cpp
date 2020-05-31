#include "timer.h"

Timer::Timer(){
    totalT0 = high_resolution_clock::now();
    hasRun = false;
}

void Timer::setTime(char part){
    if(!hasRun)
        t0 = high_resolution_clock::now();
    else{
        t1 = high_resolution_clock::now();
        durations[part] += duration_cast<nanoseconds>(t1 - t0).count();
    }

    hasRun = !hasRun;
}

void Timer::stop(){
    totalT1 = high_resolution_clock::now();
    durations[6] = duration_cast<nanoseconds>(totalT1 - totalT0).count();
}

int64_t* Timer::getPointer(){
    return durations;
}

double Timer::getConstructionTime(){
    return (double)durations[0]/1000000000;
}

double Timer::getSwapTime(){
    return (double)durations[1]/1000000000;
}

double Timer::getRevertTime(){
    return (double)durations[2]/1000000000;
}

double Timer::getReinsertion1Time(){
    return (double)durations[3]/1000000000;
}

double Timer::getReinsertion2Time(){
    return (double)durations[4]/1000000000;
}

double Timer::getReinsertion3Time(){
    return (double)durations[5]/1000000000;
}

double Timer::getTotalTime(){
    return (double)durations[6]/1000000000;
}