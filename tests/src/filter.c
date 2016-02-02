#include "test.h"
#include "kf.h"

#include <math.h>

int test(void)
{
	kf_t f = {};

	assert(!kfCreateFilter(&f, 1));
	Log("Filter structures allocated", 1);		

	f.matF[0][0] = 1;
	f.matH[0][0] = 1;
	f.matB[0][0] = 0; // no input controls

	// start with 100% uncertainty
	f.epoch[0].matP[0][0] = 1;
	f.epoch[0].state[0] = 0;	
	f.epoch[1].matP[0][0] = 1;	
	f.epoch[1].state[0] = 0;	


	float target = 2.0f;
	float t = 0, dt = 0.01;
	float state = 0;
	float dx = 1;
	while(fabs(dx) > 0.00001){
		float m = target;
		float lastState = state;
		state = 0;
		assert(!kfPredict(&f, NULL));
		assert(!kfUpdate(&f, &state, &m));

		dx = lastState - state;

		Log("X_hat = %f", 1, state);
	}

	return 0;
}

TEST_BEGIN
	.name = "Filter Test",
	.description = "Run the f through a simple use case.",
	.run = test,
TEST_END
