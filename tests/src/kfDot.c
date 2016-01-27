#include "test.h"
#include "kfMath.h"

int test(void)
{
	float xBasis[] = { 1, 0 };
	float yBasis[] = { 0, 1 };

	assert(kfDot(xBasis, xBasis, 2) == 1);
	assert(kfDot(yBasis, xBasis, 2) == 0);
	
	return 0;
}

TEST_BEGIN
	.name = "kfDot()",
	.description = "Tests validity of inner product function.",
	.run = test,
TEST_END
