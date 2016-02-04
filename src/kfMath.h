#ifndef KALMAN_FILTER_MATH
#define KALMAN_FILTER_MATH

#ifndef KALMAN_BIG_HEADER
#include "kf.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef float** kfMat_t;

#ifdef __cplusplus
extern "C" {
#endif

extern void kfVecAdd(float* r, float* v1, float* v2, int dims);
extern void kfVecSub(float* r, float* v1, float* v2, int dims);
extern float kfDot(float* v1, float* v2, int len);
extern void kfMatPrint(kfMat_t M, int dims);
extern kfMat_t kfMatAlloc(int dims);
extern kfMat_t kfMatWithCols(float* cols, int dims);
extern void kfMatCpy(float** R, float** M, int dims);
extern void kfMatScl(float** R, float** M, float s, int dims);
extern void kfMatTranspose(float** R, float** M, int dims);
extern float kfMat2SubDet(float** M, int colOff, int rowOff, int dims);
extern float kfMat2Det(float** M);
extern float kfMat3Det(float** M);
extern void kfMatIdent(float** M, int dims);
extern void kfMatAdd(float** R, float** M, float** N, int dims);
extern void kfMatSub(float** R, float** M, float** N, int dims);
extern float kfMatRowCol(float** M, float** N, int row, int col, int len);
extern void kfMatMulVec(float* r, float** M, float* v, int dims);
extern void kfMatMul(float** R, float** M, float** N, int dims);
extern int kfMat1Inverse(float** R, float** M, float** t);
extern int kfMat2Inverse(float** R, float** M, float** t);
extern int kfMat3Inverse(float** R, float** M, float** t);

#ifdef __cplusplus
}
#endif

#endif
