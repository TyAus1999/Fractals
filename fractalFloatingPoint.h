#pragma once
#include "inc.h"

typedef struct fp328{
    char sign;//1=neg, 0=pos
    long exponent;
    u64 sigFig[4];
}fp328;

/*
Multiply floating point
6.02*10^23
5.02*10^-20

n=5.02*6.02
e=23+(-20)

n*10^e
*/

void makefp328(fp328 *n, char sign, long expo, u64 number){
    n->sign=sign;
    n->exponent=expo;
    n->sigFig[0]=number;
    for(char i=3;i>0;i--)
        n->sigFig[i]=0;
}

void printfp328(fp328 *n){
    printf("%s",(n->sign)?"-":"");
    for(int i=3;i>=0;i--)
        printf("%16llX",n->sigFig[i]);
    printf("*10^%li\n",n->exponent);
}

void fp328Add(fp328 *dest, fp328 *source){
    u64 temp;
    for(int i=0;i<4;i++){
        temp=dest->sigFig[i]+source->sigFig[i];
        if(temp<dest->sigFig[i] || temp<source->sigFig[i]){
            if(i+1<4){
                dest->sigFig[i]=temp;
                dest->sigFig[i+1]++;
            }
        }
    }
}

void fp328Mul(fp328 *dest, fp328 *source){
    
}

void fpTest(){
    fp328 a,b;
    makefp328(&a,0,0,(u64)-1);
    makefp328(&b,0,0,2);
    a.sigFig[1]=(u64)-1;
    printfp328(&a);
    printfp328(&b);
    printf("===========================================\n");
    fp328Add(&a,&b);
    printfp328(&a);
    /*
    u64 i=(unsigned)-1;
    printf("%llX\n",i);
    u64 i2=i*(u64)6;
    printf("%llX\n",i2);
    printf("%llX\n",i-i2);
    */
}