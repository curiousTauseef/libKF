#include "kf.h"
#include "kfMath.h"

#include <strings.h>

int kfPredict(kf_t* f, float* c)
{
	kf_epoch_t* e_1 = f->epoch + ((f->index + 1) % 2);

	// P_t-1 =  (F_t * (P_t-1 * F_t^T)_ + Q_t
	kfMatTranspose(f->matTemp[0], f->matF);
	kfMatMul(f->matTemp[1], e_1->matP, f->matTemp[0]);
	kfMatMul(f->matTemp[0], f->matF, f->matTemp[1]);
	kfMatAdd(e_1->matP, f->matTemp[0], f->matQ);

	// update x_hat estimation for the last epoch
	kfMatMulVec(f->vecTemp[0], f->matF, e_1->state, f->dims);

	if(c){
		kfMatMulVec(f->vecTemp[1], f->matB, c, f->dims);
	}
	else{
		bzero(f->vecTemp[1], sizeof(float) * f->dims);
	}

	// update the system state estimate that will be used in the update
	// step of the filter
	kfVecAdd(e_1->state, f->vecTemp[0], f->vecTemp[1], f->dims);

	return 0;
}
