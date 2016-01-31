#ifndef KALMAN_FILTER
#define KALMAN_FILTER

#include <sys/types.h>
#include <inttypes.h>

enum{
	KF_ERR_ALLOC    = -1,
	KF_OUT_OF_RANGE = -2,
	KF_UNDEFINED    = -3,
} kf_err;

typedef struct{
	float*  state;         // 'x' in most texts
	float** matVarCovar;   // 'P' Variance and covariance matrix for each dimension
} kf_epoch_t;

typedef struct{
	uint32_t dims;
	uint32_t index;
	kf_epoch_t epoch[2];   // t, t-1 [0, 1]

	float** matStateTrans; // 'F' State transition matrix ( user defined )
	float** matCtrlInput;  // 'B' Control input matrix, maps effect of ( user defined )
	float** matTrans;      // 'H' Transition matrix maps measurements into the state domain ( user defined )
	float** matTransTrans;
	float** matMesCovars;  // 'R' Measurement covariances ( alg. defined )
	float** matKalmanGain; // 'K'                         ( alg. defined )

	float** matTemp[3]; // scratch space for whatever
	float*  vecTemp[2]; // vector scratch space
} kf_t;

int  kfCreateFilter(kf_t* filter, int dimensions);

// accepts measurement, and control vectors and
// updates relevant matrices;
void kfPredict(kf_t* filter, float* controls);

// returns a pointer to the most recent state vector estimate
void kfUpdate(kf_t* filter, float* state, float* measurements);

#endif
