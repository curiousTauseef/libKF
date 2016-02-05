#include "test.h"
#include "kf.h"

#include <indicurses.h>
#include <math.h>

void setup(void)
{
	icInit();
}

void teardown(void)
{
	icClose();
}

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

	int samples[100] = {};
	int si = 0;

	float target = 2.0f;
	float t = 0, dt = 0.01;
	float state = 0;
	float dx = 1;

	int topLeft[2] = { 5, 2 };
	int bottomRight[2] = { IC_TERM_WIDTH - 5, IC_TERM_HEIGHT - 2};

	while(fabs(dx) > 0.00001){
		float m = target;
		float lastState = state;
		state = 0;
		assert(!kfPredict(&f, NULL));
		assert(!kfUpdate(&f, &state, &m));

		dx = lastState - state;

		samples[si++] = state * 100;
		icLineGraph(topLeft, bottomRight, '*', samples, 100);

		si %= 100;

		icPresent();
		// Log("X_hat = %f", 1, state);
	}

	return 0;
}

TEST_BEGIN
	.name = "Filter Test",
	.description = "Run the f through a simple use case.",
	.run = test,
	.setup = setup,
	.teardown = teardown,
TEST_END
