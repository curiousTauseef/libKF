#ifndef KALMAN_FILTER_MATH
#define KALMAN_FILTER_MATH

#ifndef KALMAN_BIG_HEADER
#include "kf.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void kfVecAdd(float* r, float* v1, float* v2, int dims);
extern void kfVecSub(float* r, float* v1, float* v2, int dims);
extern float kfDot(float* v1, float* v2, int len);
extern void kfMatPrint(kfMat_t M);
extern kfMat_t kfMatAlloc(int cols, int rows);
extern kfMat_t kfMatWithCols(float* col, int cols, int rows);
extern void kfMatCpy(kfMat_t R, kfMat_t M);
extern void kfMatScl(kfMat_t R, kfMat_t M, float s);
extern void kfMatTranspose(kfMat_t R, kfMat_t M);
extern float kfMat2SubDet(kfMat_t M, int colOff, int rowOff);
extern float kfMat2Det(kfMat_t M);
extern float kfMat3Det(kfMat_t M);
extern void kfMatIdent(kfMat_t M);
extern void kfMatAdd(kfMat_t R, kfMat_t M, kfMat_t N);
extern void kfMatSub(kfMat_t R, kfMat_t M, kfMat_t N);
extern float kfMatRowCol(kfMat_t M, kfMat_t N, int row, int col, int len);
extern void kfMatMulVec(float* r, kfMat_t M, float* v, int dims);
extern void kfMatMul(kfMat_t R, kfMat_t M, kfMat_t N);
extern int kfMat1Inverse(kfMat_t R, kfMat_t M, kfMat_t t);
extern int kfMat2Inverse(kfMat_t R, kfMat_t M, kfMat_t t);
extern int kfMat3Inverse(kfMat_t R, kfMat_t M, kfMat_t t);

#ifdef __cplusplus
}
#endif

#endif
