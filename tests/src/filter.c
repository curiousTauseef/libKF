#include "test.h"
#include "kf.h"

int test(void)
{
	kf_t f = {};

	assert(!kfCreateFilter(&f, 1));
	Log("Filter structures allocated", 1);		

	f.matStateTrans[0][0] = 1;
	f.matTrans[0][0]      = 1;
	f.matCtrlInput[0][0]  = 0; // no input controls

	// start with 100% uncertainty
	f.epoch[0].matVarCovar[0][0] = 1;	
	f.epoch[1].matVarCovar[0][0] = 1;	


	float target = 40.0f;
	float dt = 0.01;
	float state = 0;
	for(float t = 0; t < 1; t += dt){
		float m = target;
		state = 0;
		assert(!kfPredict(&f, NULL));
		assert(!kfUpdate(&f, &state, &m));

		Log("X_hat = %f", 1, state);
	}

	return 0;
}

TEST_BEGIN
	.name = "Filter Test",
	.description = "Run the f through a simple use case.",
	.run = test,
TEST_END
