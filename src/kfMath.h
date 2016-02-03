#ifndef KALMAN_FILTER_MATH
#define KALMAN_FILTER_MATH

#ifndef KALMAN_BIG_HEADER
#include "kf.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef float** kfMat_t;

void kfVecAdd(float* r, float* v1, float* v2, int dims);
void kfVecSub(float* r, float* v1, float* v2, int dims);
float kfDot(float* v1, float* v2, int len);
void kfMatPrint(kfMat_t M, int dims);
kfMat_t kfMatAlloc(int dims);
kfMat_t kfMatWithCols(float* cols, int dims);
void kfMatCpy(float** R, float** M, int dims);
void kfMatScl(float** R, float** M, float s, int dims);
void kfMatTranspose(float** R, float** M, int dims);
float kfMat2SubDet(float** M, int colOff, int rowOff, int dims);
float kfMat2Det(float** M);
float kfMat3Det(float** M);
void kfMatIdent(float** M, int dims);
void kfMatAdd(float** R, float** M, float** N, int dims);
void kfMatSub(float** R, float** M, float** N, int dims);
float kfMatRowCol(float** M, float** N, int row, int col, int len);
void kfMatMulVec(float* r, float** M, float* v, int dims);
void kfMatMul(float** R, float** M, float** N, int dims);
int kfMat1Inverse(float** R, float** M, float** t);
int kfMat2Inverse(float** R, float** M, float** t);
int kfMat3Inverse(float** R, float** M, float** t);

#endif
