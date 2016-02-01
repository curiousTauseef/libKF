#include "kf.h"
#include "kfMath.h"

#include <strings.h>

int kfPredict(kf_t* f, float* c)
{
	kf_epoch_t* e_1 = f->epoch + ((f->index + 1) % 2);
	kf_epoch_t* e_0 = f->epoch + f->index;

	// P_t = P_t-1 - K_t * H_t * P_t-1
	kfMatMul(f->matTemp[0], f->matTrans, e_1->matVarCovar, f->dims);
	kfMatMul(f->matTemp[1], f->matKalmanGain, f->matTemp[0], f->dims);
	kfMatSub(e_0->matVarCovar, e_1->matVarCovar, f->matTemp[1], f->dims);

	// update x_hat estimation for the last epoch
	kfMatMulVec(f->vecTemp[0], f->matStateTrans, e_1->state, f->dims);

	if(c){
		kfMatMulVec(f->vecTemp[1], f->matCtrlInput, c, f->dims); 
	}
	else{
		bzero(f->vecTemp[1], sizeof(float) * f->dims);
	}	

	// update the system state estimate that will be used in the update
	// step of the filter
	kfVecAdd(e_0->state, f->vecTemp[0], f->vecTemp[1], f->dims);

	return 0;
}
