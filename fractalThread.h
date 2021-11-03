#pragma once
#include "inc.h"

typedef struct fractalThreadFuncArgs{
    mtex *mutex;
    double minViewX,maxViewX;
    double minViewY,maxViewY;
    int startX,endX;
    int startY,endY;
    int currentWidth, currentHeight;
    unsigned char *pixels;
    int yAddOffset;
    int startingY;
} fractalThreadFuncArgs;

unsigned getIndexThread(int x, int y, fractalThreadFuncArgs *a){
    int newX=x+a->endX;
    int newY=y+a->endY;
    unsigned out=newY*a->currentHeight;
    out+=newX;
    out*=3;
    return out;    
}

void *fractalThreadFunc(void *a){
    u64 maxCount=0x2fd;
    fractalThreadFuncArgs *args=(fractalThreadFuncArgs*)a;
    for(int y=args->startingY;y<args->endY;y+=args->yAddOffset){
        for(int x=args->startX;x<args->endX;x++){
            double mappedX=map((double)x, (double)args->startX, (double)args->endX, (double)args->minViewX, (double)args->maxViewX);
            double mappedY=map((double)y, (double)args->startY, (double)args->endY, (double)args->minViewY, (double)args->maxViewY);
            u64 count=fractalModifiedNonOp(mappedX, mappedY, maxCount);
            unsigned index=getIndexThread(x,y,args);
            lockMutex(args->mutex);
            unsigned char *w=args->pixels;
            u64 colourSelection=count/(u64)255;
            double colourAmount=(double)count/255.0;
            colourAmount-=(double)colourSelection;
            colourAmount*=255.0;
            for(char i=0;i<3;i++)
                w[index+i]=0;
            w[index+colourSelection]=(unsigned char)colourAmount;
            pthread_mutex_unlock(args->mutex);
        }
    }
    printf("Thread exit\n");
    return 0;
}

int startThreadsFractal(mtex *lock, unsigned char *pixels, int threads, 
int startX, int endX, int startY, int endY, int currentWidth, int currentHeight,
double viewXMin, double viewXMax, double viewYMin, double viewYMax){
    pthread_t thid[threads];
    fractalThreadFuncArgs ag[threads];
    for(int i=0;i<threads;i++){
        fractalThreadFuncArgs *w=&ag[i];
        w->mutex=lock;
        w->minViewX=viewXMin;
        w->maxViewX=viewXMax;
        w->minViewY=viewYMin;
        w->maxViewY=viewYMax;
        w->startX=startX;
        w->endX=endX;
        w->startY=startY;
        w->endY=endY;
        w->currentWidth=currentWidth;
        w->currentHeight=currentHeight;
        w->pixels=pixels;
        w->yAddOffset=threads;
        w->startingY=startY+i;
        int tCreate=pthread_create(&thid[i],NULL,fractalThreadFunc,&ag[i]);
        if(tCreate!=0){
            printf("Thread Creation Error %i\n", tCreate);
        }
    }   
    for(int i=0;i<threads;i++){
        int threadReturn;
        void *threadRet;
        threadReturn=pthread_join(thid[i], &threadRet);
        if(threadReturn!=0){
            printf("Thread Error: %i\n", threadReturn);
        }
    }
    return 0;
}