#pragma once
#include "inc.h"

//f(z)=z*z + (c+y*i)
//c is pretty much the x component

char fractalNonOptimized(double x, double y, u64 maxTestDepth){
    //ans must be <=2
    double z=0;
    double working;
    double c=x+y;
    for(u64 i=0;i<maxTestDepth;i++){
        working=z*z;
        working+=c;
        z=working;
        if(z>2 || z <-2)
            return 0;
    }
    return 1;
}

typedef struct complexNumber{
    double a,bi;
} complexNumber;

void printComplexNumber(complexNumber *number){
    printf("Real: %f\t\tImaginary: %f\n", number->a, number->bi);
}

void complexNumberAdd(complexNumber *a, complexNumber *b, complexNumber *ans){
    ans->a=a->a+b->a;
    ans->bi=a->bi+b->bi;
}

void complexNumberMul(complexNumber *a, complexNumber *b, complexNumber *ans){
    double terms[4];
    terms[0]=a->a*b->a;//real
    terms[1]=a->a*b->bi;//imag
    terms[2]=a->bi*b->a;//imag
    terms[3]=a->bi*b->bi;//imag^2

    terms[3]*=-1;
    ans->a=terms[0]+terms[3];
    ans->bi=terms[1]+terms[2];
}

void complexNumberMov(complexNumber *dest, complexNumber *source){
    dest->a=source->a;
    dest->bi=source->bi;
}

//Returns the count
u64 fractalModifiedNonOp(double x, double y, u64 maxCount){
    complexNumber c,z,tempZ;
    c.a=x;
    c.bi=y;
    z.a=0;
    z.bi=0;
    double temp;
    for(u64 i=0;i<maxCount;i++){
        complexNumberMul(&z,&z,&z);
        complexNumberAdd(&z,&c,&tempZ);
        complexNumberMov(&z,&tempZ);
        temp=(z.a*z.a)+(z.bi*z.bi);
        if(temp>=4)
            return i;
    }
    return maxCount;
}