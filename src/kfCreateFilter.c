#include "kf.h"
#include "kfMath.h"

#include <stdlib.h>
#include <unistd.h>

static kfMat_t allocMat(int cols, int rows)
{
	kfMat_t M = {
		.cols = cols,
		.rows = rows,
		.col = (float**)malloc(sizeof(float*) * cols)
	};

	if(!M.col){
		bzero(&M, sizeof(kfMat_t));
		return M;
	}

	for(int i = cols; i--;){
		M.col[i] = (float*)malloc(sizeof(float) * rows);
		if(!M.col[i]){
			bzero(&M, sizeof(kfMat_t));
			return M;
		}
	}

	return M;
}

int kfCreateFilter(kf_t* filter, int dims)
{
	// allocate state vectors and variance/covariance matrices
	// for the two recursive time epochs
	for(int i = 2; i--;){
		kf_epoch_t* e = filter->epoch + i;
		e->state = (float*)malloc(sizeof(float) * dims);
		e->matP = allocMat(dims, dims);

		if(!e->state || !e->matP.col) return KF_ERR_ALLOC;

		kfMatIdent(e->matP);
	}

	filter->index = 0;
	filter->dims  = dims;

	// allocate all epoch independent matrices
	kfMat_t* mats = &filter->matF;
	for(int i = 0; (void*)(mats + i) != (void*)filter->matTemp; ++i){
		mats[i] = allocMat(dims, dims);
		kfMatIdent(mats[i]);
		if(!mats[i].col) return KF_ERR_ALLOC;
	}

	// allocate temp matrices
	for(int i = 3; i--;){
		filter->matTemp[i] = allocMat(dims, dims);
		kfMatIdent(filter->matTemp[i]);
		if(!filter->matTemp[i].col) return KF_ERR_ALLOC;
	}

	// allocate temp vectors
	for(int i = 2; i--;){
		filter->vecTemp[i] = malloc(sizeof(float) * dims);
		if(!filter->vecTemp[i]) return KF_ERR_ALLOC;
	}

	return 0;
}
