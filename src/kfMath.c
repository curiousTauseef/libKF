#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "kfMath.h"

void kfVecAdd(float* r, float* v1, float* v2,int dims)
{
	for(;dims--;){
		r[dims] = v1[dims] + v2[dims];
	}
}
//-----------------------------------------------------------------------------
void kfVecSub(float* r, float* v1, float* v2,int dims)
{
	for(;dims--;){
		r[dims] = v1[dims] - v2[dims];
	}
}
//-----------------------------------------------------------------------------
void kfVecCross(float* r, float* v1, float* v2, int dims)
{
	r[0] = v1[1] * v2[2] - v1[2] * v2[1];
	r[1] = v1[2] * v2[0] - v1[0] * v2[2];
	r[2] = v1[0] * v2[1] - v1[1] * v2[0];
}
//-----------------------------------------------------------------------------
float kfDot(float* v1, float* v2,int len)
{
	float d = 0;
	for(;len--; d += v1[len] * v2[len]);
	return d;
}
//-----------------------------------------------------------------------------
void kfMatPrint(kfMat_t M)
{
	int maxRowLen = 0;

	for(int i = 0; i < M.rows; ++i){
		char buf[128] = {};
		int row = 0;
		for(int j = 0; j < M.cols; ++j){
			int l = sprintf(buf, " %0.3f ", M.col[j][i]);
			row += l;
		}
		if(row > maxRowLen) maxRowLen = row;
	}

	for(int i = 0; i < M.rows; ++i){
		char buf[128] = {};
		char* off = buf;
		int thisRowLen = 0;

		for(int j = 0; j < M.cols; ++j){
			int chars = sprintf(off, " %0.3f ", M.col[j][i]);
			off += chars;
			thisRowLen += chars;
		}

		printf("|");
		for(int j = 0; j < maxRowLen; ++j){
			printf(" ");
		}
		printf("|\n");

		printf("|%s", buf);
		for(int j = 0; j < (maxRowLen - thisRowLen); ++j){
			printf(" ");
		}
		printf("|\n");

		printf("|");
		for(int j = 0; j < maxRowLen; ++j){
			printf(" ");
		}
		printf("|\n");
	}
}
//-----------------------------------------------------------------------------
kfMat_t kfMatAlloc(int cols, int rows)
{
	kfMat_t mat = {
		cols, rows,
		malloc(sizeof(float*) * cols)
	};

	for(int i = cols; i--;){
		mat.col[i] = malloc(sizeof(float) * rows);
	}

	return mat;
}
//-----------------------------------------------------------------------------
kfMat_t kfMatWithCols(float* col, int cols, int rows)
{
	kfMat_t mat = {
		cols, rows,
		malloc(sizeof(float*) * cols)
	};

	for(int i = cols; i--;){
		mat.col[i] = malloc(sizeof(float) * rows);
		memcpy(mat.col[i], col + (i * rows), sizeof(float) * rows);
	}

	return mat;
}
//-----------------------------------------------------------------------------
void kfMatCpy(kfMat_t R, kfMat_t M)
{
	for(int i = R.cols; i--;){
		for(int j = R.rows; j--;){
			R.col[i][j] = M.col[i][j];
		}
	}
}
//-----------------------------------------------------------------------------
void kfMatScl(kfMat_t R, kfMat_t M, float s)
{
	for(int i = R.cols; i--;){
		for(int j = R.rows; j--;){
			R.col[i][j] = M.col[i][j] * s;
		}
	}
}
//-----------------------------------------------------------------------------
void kfMatTranspose(kfMat_t R, kfMat_t M)
{
	for(int i = R.cols; i--;){
		for(int j = R.rows; j--;){
			R.col[i][j] = M.col[j][i];
		}
	}
}
//-----------------------------------------------------------------------------
float kfMat2SubDet(kfMat_t M,int colOff,int rowOff)
{
	int colOff1 = (colOff + 1) % M.cols;
	int rowOff1 = (rowOff + 1) % M.rows;
	return M.col[colOff][rowOff] * M.col[colOff1][rowOff1] - M.col[colOff1][rowOff] * M.col[colOff][rowOff1];
}
//-----------------------------------------------------------------------------
float kfMat2Det(kfMat_t M)
{
	return M.col[0][0] * M.col[1][1] - M.col[1][0] * M.col[0][1];
}
//-----------------------------------------------------------------------------
float kfMat3Det(kfMat_t M)
{
	return M.col[0][0] * (M.col[1][1] * M.col[2][2] - M.col[1][2] * M.col[2][1]) +
	       M.col[1][0] * (M.col[2][1] * M.col[0][2] - M.col[2][2] * M.col[0][1]) +
	       M.col[2][0] * (M.col[0][1] * M.col[1][2] - M.col[0][2] * M.col[1][1]);
}
//-----------------------------------------------------------------------------
void kfMatIdent(kfMat_t M)
{
	for(int i = M.cols; i--;){
		for(int j = M.rows; j--;){
			if(i == j)M.col[i][j] = 1;
			else      M.col[i][j] = 0;
		}
	}
}
//-----------------------------------------------------------------------------
void kfMatAdd(kfMat_t R, kfMat_t M, kfMat_t N)
{
	for(int i = R.cols; i--;){
		for(int j = R.rows; j--;){
			R.col[i][j] = M.col[i][j] + N.col[i][j];
		}
	}
}
//-----------------------------------------------------------------------------
void kfMatSub(kfMat_t R, kfMat_t M, kfMat_t N)
{
	for(int i = R.cols; i--;){
		for(int j = R.rows; j--;){
			R.col[i][j] = M.col[i][j] - N.col[i][j];
		}
	}
}
//-----------------------------------------------------------------------------
float kfMatRowCol(kfMat_t M, kfMat_t N,int row,int col,int len)
{
	float d = 0;
	for(int i = len; i--;){
		d += M.col[i][row] * N.col[col][i];
	}
	return d;
}
//-----------------------------------------------------------------------------
// TODO continue from here
void kfMatMulVec(float* r, kfMat_t M, float* v,int dims)
{
	kfMat_t vec = {
		.cols = 1,
		.rows = dims,
		.col = &v
	};

	for(int i = dims; i--;){
		r[i] = kfMatRowCol(M, vec, i, 0, dims);
	}
}
//-----------------------------------------------------------------------------
void kfMatMul(kfMat_t R, kfMat_t M, kfMat_t N)
{
	for(int i = M.rows; i--;){
		for(int j = N.cols; j--;){
			R.col[j][i] = kfMatRowCol(M, N, i, j, M.rows);
		}
	}
}
//-----------------------------------------------------------------------------
int kfMat1Inverse(kfMat_t R, kfMat_t M, kfMat_t t)
{
	if(M.col[0][0] == 0) return KF_UNDEFINED;

	R.col[0][0] = 1.0f / M.col[0][0];

	return 0;
}
//-----------------------------------------------------------------------------
int kfMat2Inverse(kfMat_t R, kfMat_t M, kfMat_t t)
{
	float d = kfMat2Det(M);
	if(d == 0) return KF_UNDEFINED;

	R.col[0][0] =  M.col[1][1]; R.col[1][0] = -M.col[1][0];
	R.col[0][1] = -M.col[0][1]; R.col[1][1] =  M.col[0][0];

	kfMatScl(R, R, 1.0f / d);

	return 0;
}
//-----------------------------------------------------------------------------
int kfMat3Inverse(kfMat_t R, kfMat_t M, kfMat_t t)
{
	float d = kfMat3Det(M);
	if(d == 0) return KF_UNDEFINED;

	kfMatTranspose(t, M);

	R.col[0][0] = t.col[1][1] * t.col[2][2] - t.col[2][1] * t.col[1][2];
	R.col[1][0] = t.col[0][1] * t.col[2][2] - t.col[2][1] * t.col[0][2];
	R.col[2][0] = t.col[0][1] * t.col[1][2] - t.col[1][1] * t.col[0][2];

	R.col[0][1] = t.col[1][0] * t.col[2][2] - t.col[2][0] * t.col[1][2];
	R.col[1][1] = t.col[0][0] * t.col[2][2] - t.col[2][0] * t.col[0][2];
	R.col[2][1] = t.col[0][0] * t.col[1][2] - t.col[1][0] * t.col[0][2];

	R.col[0][2] = t.col[1][0] * t.col[2][1] - t.col[2][0] * t.col[1][1];
	R.col[1][2] = t.col[0][0] * t.col[2][1] - t.col[0][1] * t.col[2][0];
	R.col[2][2] = t.col[0][0] * t.col[1][1] - t.col[1][0] * t.col[0][1];

	// per element multiply with adj matrix
	for(int i = 3; i--;){
		for(int j = 3; j--;){
			R.col[j][i] = R.col[j][i] * ((i + j) % 2 ? -1 : 1) / d;
		}
	}

	return 0;
}
