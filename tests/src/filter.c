#include "test.h"
#include "kf.h"

int test(void)
{
	kf_t filter = {};

	assert(!kfCreateFilter(&filter, 1));
	Log("Filter structures allocated", 1);		

	float dt = 0.01;
	float state = 0;
	for(float t = 0; t < 1; t += dt){
		float m = t;
		assert(!kfPredict(&filter, NULL));
		assert(!kfUpdate(&filter, &state, &m));
	}

	return 0;
}

TEST_BEGIN
	.name = "Filter Test",
	.description = "Run the filter through a simple use case.",
	.run = test,
TEST_END
