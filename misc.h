#pragma once
#include "inc.h"

double map(double value, double valueMin, double valueMax, double mapMin, double mapMax){    
    double slope=(mapMax-mapMin)/(valueMax-valueMin);
    return mapMin+(slope*(value-valueMin));
}

void lockMutex(mtex *lock){
    int lockValue=pthread_mutex_lock(lock);
    while(lockValue==-1){
        sleep(0.1);
        lockValue=pthread_mutex_lock(lock);
    }
}