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

void kfVecSub(float* r, float* v1, float* v2,int dims)
{
	for(;dims--;){
		r[dims] = v1[dims] - v2[dims];
	}
}

float kfDot(float* v1, float* v2,int len)
{
	float d = 0;
	for(;len--; d += v1[len] * v2[len]);
	return d;
}

void kfMatPrint(kfMat_t M,int dims)
{
	int maxRowLen = 0;

	for(int i = 0; i < dims; ++i){
		char buf[128] = {};
		int row = 0;
		for(int j = 0; j < dims; ++j){
			int l = sprintf(buf, " %0.3f ", M[j][i]);
			row += l;
		}
		if(row > maxRowLen) maxRowLen = row;
	}

	for(int i = 0; i < dims; ++i){
		char buf[128] = {};
		char* off = buf;
		int thisRowLen = 0;

		for(int j = 0; j < dims; ++j){
			int chars = sprintf(off, " %0.3f ", M[j][i]);
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

kfMat_t kfMatAlloc(int dims)
{
	kfMat_t mat = malloc(sizeof(float*) * dims);

	for(int i = dims; i--;){
		mat[i] = malloc(sizeof(float) * dims);
	}

	return mat;
}

kfMat_t kfMatWithCols(float* cols,int dims)
{
	kfMat_t mat = malloc(sizeof(float*) * dims);

	for(int i = dims; i--;){
		mat[i] = malloc(sizeof(float) * dims);
		memcpy(mat[i], cols + (i * dims), sizeof(float) * dims);
	}

	return mat;
}

void kfMatCpy(float** R, float** M,int dims)
{
	for(int i = dims; i--;){
		for(int j = dims; j--;){
			R[i][j] = M[i][j];
		}
	}
}

void kfMatScl(float** R, float** M, float s,int dims)
{
	for(int i = dims; i--;){
		for(int j = dims; j--;){
			R[i][j] = M[i][j] * s;
		}
	}
}

void kfMatTranspose(float** R, float** M,int dims)
{
	for(int i = dims; i--;){
		for(int j = dims; j--;){
			R[i][j] = M[j][i];
		}
	}
}

float kfMat2SubDet(float** M,int colOff,int rowOff,int dims)
{
	int colOff1 = (colOff + 1) % dims;
	int rowOff1 = (rowOff + 1) % dims;
	return M[colOff][rowOff] * M[colOff1][rowOff1] - M[colOff1][rowOff] * M[colOff][rowOff1];
}

float kfMat2Det(float** M)
{
	return M[0][0] * M[1][1] - M[1][0] * M[0][1];
}

float kfMat3Det(float** M)
{
	return M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) +
	       M[1][0] * (M[2][1] * M[0][2] - M[2][2] * M[0][1]) +
	       M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1]);
}

void kfMatIdent(float** M,int dims)
{
	for(int i = dims; i--;){
		for(int j = dims; j--;){
			if(i == j)M[i][j] = 1;
			else      M[i][j] = 0;
		}
	}
}

void kfMatAdd(float** R, float** M, float** N,int dims)
{
	for(int i = dims; i--;){
		for(int j = dims; j--;){
			R[i][j] = M[i][j] + N[i][j];
		}
	}
}

void kfMatSub(float** R, float** M, float** N,int dims)
{
	for(int i = dims; i--;){
		for(int j = dims; j--;){
			R[i][j] = M[i][j] - N[i][j];
		}
	}
}

 float kfMatRowCol(float** M, float** N,int row,int col,int len)
{
	float d = 0;
	for(int i = len; i--;){
		d += M[i][row] * N[col][i];
	}
	return d;
}

void kfMatMulVec(float* r, float** M, float* v,int dims)
{
	for(int i = dims; i--;){
		r[i] = kfMatRowCol(M, &v, i, 0, dims);
	}
}

void kfMatMul(float** R, float** M, float** N,int dims)
{
	for(int i = dims; i--;){
		for(int j = dims; j--;){
			R[j][i] = kfMatRowCol(M, N, i, j, dims);
		}
	}
}

int kfMat1Inverse(float** R, float** M, float** t)
{
	if(M[0][0] == 0) return KF_UNDEFINED;

	R[0][0] = 1.0f / M[0][0];

	return 0;
}

int kfMat2Inverse(float** R, float** M, float** t)
{
	float d = kfMat2Det(M);
	if(d == 0) return KF_UNDEFINED;

	R[0][0] =  M[1][1]; R[1][0] = -M[1][0];
	R[0][1] = -M[0][1]; R[1][1] =  M[0][0];


	kfMatScl(R, R, 1.0f / d, 2);

	return 0;
}

int kfMat3Inverse(float** R, float** M, float** t)
{
	float d = kfMat3Det(M);
	if(d == 0) return KF_UNDEFINED;

	kfMatTranspose(t, M, 3);

	R[0][0] = t[1][1] * t[2][2] - t[2][1] * t[1][2];
	R[1][0] = t[0][1] * t[2][2] - t[2][1] * t[0][2];
	R[2][0] = t[0][1] * t[1][2] - t[1][1] * t[0][2];

	R[0][1] = t[1][0] * t[2][2] - t[2][0] * t[1][2];
	R[1][1] = t[0][0] * t[2][2] - t[2][0] * t[0][2];
	R[2][1] = t[0][0] * t[1][2] - t[1][0] * t[0][2];

	R[0][2] = t[1][0] * t[2][1] - t[2][0] * t[1][1];
	R[1][2] = t[0][0] * t[2][1] - t[0][1] * t[2][0];
	R[2][2] = t[0][0] * t[1][1] - t[1][0] * t[0][1];

	// per element multiply with adj matrix
	for(int i = 3; i--;){
		for(int j = 3; j--;){
			R[j][i] = R[j][i] * ((i + j) % 2 ? -1 : 1) / d;
		}
	}

	return 0;
}
