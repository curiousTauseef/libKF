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
	float** matP;   // 'P' Variance and covariance matrix for each dimension
} kf_epoch_t;

typedef struct{
	uint32_t dims;
	uint32_t index;
	kf_epoch_t epoch[2];   // t, t-1 [0, 1]

	float** matF;   // 'F' State transition matrix ( user defined )
	float** matB;   // 'B' Control input matrix, maps effect of control input onto the state ( user defined )
	float** matH;   // 'H' Transition matrix maps measurements into the state domain ( user defined )
	float** matH_T;
	float** matR;   // 'R' Measurement covariances ( alg. defined )
	float** matK;   // 'K'                         ( alg. defined )

	float** matTemp[3]; // scratch space for whatever
	float*  vecTemp[2]; // vector scratch space
} kf_t;

#ifdef __cplusplus
extern "C" {
#endif

extern int kfCreateFilter(kf_t* filter, int dimensions);

// accepts measurement, and control vectors and
// updates relevant matrices;
extern int kfPredict(kf_t* filter, float* controls);

// returns a pointer to the most recent state vector estimate
extern int kfUpdate(kf_t* filter, float* state, float* measurements);

#ifdef __cplusplus
}
#endif

#endif
