#include "kf.h"
#include "kfMath.h"

#include <stdlib.h>
#include <unistd.h>

typedef int(*kfMatInv_f)(float**, float**, float**);

int kfUpdate(kf_t* f, float* state, float* z)
{
	int(*inv[])(float**, float**, float**) = {
		NULL,
		kfMat1Inverse,
		kfMat2Inverse,
		kfMat3Inverse
	};

	int d = f->dims;
	kf_epoch_t* e_1 = f->epoch + ((f->index + 1) % 2);
	kf_epoch_t* e_0 = f->epoch + f->index;

	// H_t^T
	kfMatTranspose(f->matH_T, f->matH, d);

	// S_t = H_t * P_t-1 * H_t^T + R_t
	kfMatMul(f->matTemp[1], e_1->matP, f->matH_T, d);
	kfMatMul(f->matTemp[0], f->matH, f->matTemp[1], d);
	kfMatAdd(f->matTemp[1], f->matTemp[0], f->matR, d);

	// S_t^-1
	if(inv[d](f->matTemp[0], f->matTemp[1], f->matTemp[2])){
		return KF_UNDEFINED; // error, not invertible
	}

	// K_t = P_t-1 * H_t^T * S_t^-1
	kfMatMul(f->matTemp[1], f->matH_T, f->matTemp[0], d);
	kfMatMul(f->matK, e_1->matP, f->matTemp[1], d);

	// X_t = X_t-1 + K_t * (z - H_t-1 * X_t-1)
	kfMatMulVec(f->vecTemp[0], f->matH, e_1->state, d);
	kfVecSub(f->vecTemp[1], z, f->vecTemp[0], d);
	kfMatMulVec(f->vecTemp[0], f->matK, f->vecTemp[1], d);
	kfVecAdd(e_0->state, e_1->state, f->vecTemp[0], d);
	for(int i = d; i--; state[i] = e_0->state[i]);

	// update the covariance matrix for this epoch
	// P_t = P_t-1 - K_t * H_t * P_t-1
	kfMatMul(f->matTemp[0], f->matH, e_1->matP, d);     // H_t * P_t-1
	kfMatMul(f->matTemp[1], f->matK, f->matTemp[0], d); // K_t * (H_t * P_t-1)
	kfMatSub(e_0->matP, e_1->matP, f->matTemp[1], d);   // P_t = P_t-1 - K_t * (H_t * P_t-1)

	// roll over to the next epoch
	++f->index;
	f->index %= 2;

	return 0;
}
