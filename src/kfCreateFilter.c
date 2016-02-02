#include "kf.h"
#include "kfMath.h"

#include <stdlib.h>
#include <unistd.h>

static float** allocMat(int cols, int rows)
{
	float** mat = (float**)malloc(sizeof(float*) * cols);
	if(!mat) return NULL;

	for(int i = cols; i--;){
		mat[i] = (float*)malloc(sizeof(float) * rows);
		if(!mat[i]) return NULL;
	}		

	return mat;
}

int kfCreateFilter(kf_t* filter, int dims)
{
	// allocate state vectors and variance/covariance matrices
	// for the two recursive time epochs
	for(int i = 2; i--;){
		kf_epoch_t* e = filter->epoch + i;
		e->state = (float*)malloc(sizeof(float));
		e->matP = allocMat(dims, dims);
	
		if(!e->state || !e->matP) return KF_ERR_ALLOC;
		
		kfMatIdent(e->matP, dims);
	}

	filter->index = 0;
	filter->dims  = dims;

	// allocate all epoch independent matrices
	float*** mats = &filter->matF;
	for(int i = 6; i--;){
		mats[i] = allocMat(dims, dims);
		kfMatIdent(mats[i], dims);
		if(!mats[i]) return KF_ERR_ALLOC;
	}
	
	// allocate temp matrices
	for(int i = 3; i--;){
		filter->matTemp[i] = allocMat(dims, dims);
		kfMatIdent(filter->matTemp[i], dims);
		if(!filter->matTemp[i]) return KF_ERR_ALLOC;
	}

	// allocate temp vectors	
	for(int i = 2; i--;){
		filter->vecTemp[i] = malloc(sizeof(float) * dims);
		if(!filter->vecTemp[i]) return KF_ERR_ALLOC;
	}

	return 0;
}
