#pragma once
#include "inc.h"

//All pointers are pointers to 4 length arrays that are 32 bit alligned
void fractalIntrin(double *xs, double *ys, u64 *counts, u64 maxCount){
    __m256d cx,cy;
    __m256d zx,zy;
    __m256d zTemp,neg1;
    __m256d mulTerms;
    __m256i _counts, maxCounts;
    cx=_mm256_load_pd(xs);
    cy=_mm256_load_pd(ys);
    zx=_mm256_setzero_pd();
    zy=_mm256_setzero_pd();
    _counts=_mm256_setzero_si256();
    maxCounts=_mm256_set1_epi64x(maxCount);
    neg1=_mm256_set1_pd(-1.0);

    //zTemp=_mm256_permute4x64_pd(cx, 0b00000000);
    mulTerms=_mm256_blend_pd(cx,cy,0b0011);
    

    for(int i=0;i<4;i++)
        printf("%i, %f\n", i, xs[i]);

    printf("\n");

    double *temp=aligned_alloc(32, sizeof(double)*4);
    _mm256_store_pd(temp, mulTerms);
    for(int i=0;i<4;i++)
        printf("%i, %f\n", i, temp[i]);
    free(temp);

}