#include "test.h"
#include "kfMath.h"

static float rf()
{
	return ((random() % 2048) / 1024.0f) - 1.0f;
}

int succeed(void)
{
	float up[3] = { rf(), rf(), rf() };
	float left[3] = { rf(), rf(), rf() };
	float forward[3] = { };

	kfVecCross(forward, up, left, 3);

	printf("{ %f, %f, %f } x { %f, %f, %f } = { %f, %f, %f }\n",
	up[0], up[1], up[2],
	left[0], left[1], left[2],
	forward[0], forward[1], forward[2]
	);

	float dot = kfVecDot(up, forward, 3);
	printf("dot: %f\n", dot);
	if(dot > 0.00001f){
		return -1;
	}

	return 0;
}

TEST_BEGIN
	.name = "kfVecCross()",
	.description = "Tests the orthogonality of vectors resulting from the cross prod function.",
	.run = succeed,
TEST_END
