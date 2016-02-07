#include "test.h"
#include "kf.h"

#include <indicurses.h>
#include <math.h>

void setup(void)
{

}

void teardown(void)
{
	icClose();
}

float noise(){
	return (((random() % 2048) / 1024.0f) - 1.0f) * 1.0f;
}

int test(void)
{
	float target = 2.0f;
	float t = 0, dt = 0.01;
	float state[2] = {};
	float dx = 1;

	kf_t f = {};

	assert(!kfCreateFilter(&f, 1));
	Log("Filter structures allocated", 1);

	f.matF[0][0] = 1;
	// f.matF[1][0] = dt;

	f.matH[0][0] = 1;
	// f.matH[1][1] = 1;

	f.matB[0][0] = 0; // no input controls
	// f.matB[1][1] = 0; // no input controls

	// zero mean Variance
	f.matR[0][0] = 0;
	Log("Computing Variance\n", 1);
	for(int i = 100; i--;){
		for(int j = f.dims; j--;){
			f.matR[j][j] += powf(noise(), 2);
		}
	}

	for(int j = f.dims; j--;){
		f.matR[j][j] /= 100.0f;
	}

	f.matQ[0][0] = 0.001;

	Log("Variance: %f\n", 1, f.matR[0][0]);
	sleep(1);

	// start with 100% uncertainty
	f.epoch[0].matP[0][0] = f.epoch[1].matP[0][0] = 1;
	f.epoch[0].state[0]   = f.epoch[1].state[0]   = 0;

	int stateSamples[100] = {};
	int measurements[100] = {};
	int si = 0;

	icInit();

	int topLeft[2] = { 5, 2 };
	int bottomRight[2] = { IC_TERM_WIDTH - 5, IC_TERM_HEIGHT - 2};

	int minMax[2] = { -200, 200 };

	while(1 || fabs(dx) > 0.00001){
		float reading = (sin( t ) * target) + noise();
		float m[] = {
				reading,
				reading - state[0]
		};//sin(t) * 4;// + noise();// * 0.1;
		float lastState = state[0];
		state[0] = 0;
		assert(!kfPredict(&f, NULL));
		assert(!kfUpdate(&f, state, m));

		minMax[0] = m[0] < minMax[0] ? m[0] : minMax[0];
		minMax[1] = m[0] > minMax[1] ? m[0] : minMax[1];

		dx = lastState - state[0];

		measurements[si] = m[0] * 100;
		stateSamples[si++] = state[0] * 100;

		clear();
		icLineGraph(topLeft, bottomRight, '.', measurements, 100, minMax);
		icLineGraph(topLeft, bottomRight, '*', stateSamples, 100, minMax);

		icTextf(5, 5, "CoVar = %f", f.epoch[0].matP[0][0]);
		icTextf(5, 6, "X = { %f, %f }", state[0], state[1]);

		si %= 100;
		t += 0.1;

		icPresent();
		usleep(10000);
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
