#include "kf.h"
#include "kfMath.h"

#include <stdlib.h>

typedef int(*kfMatInv_f)(float**, float**);

void kfUpdate(kf_t* f, float* state, float* m)
{
	int(*inv[])(float**, float**) = {
		NULL,
		NULL,
		kfMat2Inverse,
		kfMat3Inverse
	};

	int d = f->dims;
	kf_epoch_t* e_1 = f->epoch + ((f->index + 1) % 2);
	kf_epoch_t* e_0 = f->epoch + f->index;

	// H_k^T
	kfMatTranspose(f->matTransTrans, f->matTrans, d);

	// S_k = H_k * P_k-1 * H_k^T + R_k
	kfMatMul(f->matTemp[1], e_1->matVarCovar, f->matTransTrans, d);
	kfMatMul(f->matTemp[0], f->matTrans, f->matTemp[1], 3);
	kfMatAdd(f->matTemp[1], f->matTemp[0], f->matMesCovars, d);
	
	// S_k^-1
	if(inv[d](f->matTemp[0], f->matTemp[1])){
		return; // error, not invertible
	}

	// K_k = P_k-1 * H_k^T * S_k^-1
	kfMatMul(f->matTemp[1], f->matTransTrans, f->matTemp[0], d);
	kfMatMul(f->matKalmanGain, e_1->matVarCovar, f->matTemp[1], d);

	// X_k = X_k-1 + K_k * z_k	
	kfMatMulVec(f->vecTemp[0], f->matKalmanGain, m, d);
	kfVecAdd(state, e_1->state, f->vecTemp[0], d);

	// update the covariance matrix for this epoch
	kfMatMul(f->matTemp[0], f->matTrans, e_1->matVarCovar, d);      // H_k * P_k-1
	kfMatMul(f->matTemp[1], f->matKalmanGain, f->matTemp[0], d);    // K_k * (H_k * P_k-1)
	kfMatSub(e_0->matVarCovar, e_1->matVarCovar, f->matTemp[1], d); // P_k = P_k-1 - K_k * (H_k * P_k-1) 

	// roll over to the next epoch
	++f->index;
	f->index %= 2;
}
