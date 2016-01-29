#include "test.h"
#include "kfMath.h"

static float rf(){ return ((random() % 1024) / 512.0f) - 1.0f; }

static int test(void)
{
	float M_temp[3][3] = {
		{ rf(), rf(), rf() },
		{ rf(), rf(), rf() },
		{ rf(), rf(), rf() },
	};
	float cpy_temp[3][3] = {
		{ M_temp[0][0], M_temp[1][0], M_temp[2][0]},
		{ M_temp[0][1], M_temp[1][1], M_temp[2][1]},
		{ M_temp[0][2], M_temp[1][2], M_temp[2][2]},
	};

	kfMat_t M   = kfMatWithCols((float*)M_temp, 3);
	kfMat_t N   = kfMatWithCols((float*)M_temp, 3);
	kfMat_t cpy = kfMatWithCols((float*)cpy_temp, 3);

	kfMatPrint(M, 3);
	Log("T = ", 0);
	kfMatPrint(cpy, 3);

	Log("mat size %d, transposing...", 1, sizeof(M)); 
	kfMatTranspose(M, N, 3);
	Log("Transposed", 1, NULL);
	kfMatPrint(M, 3);

	for(int i = 3; i--;){
		for(int j = 3; j--;){
			assert(cpy_temp[i][j] == M[i][j]);
		}
	}

	return 0;
}

TEST_BEGIN
	.name = "kfMatTranspose()",
	.description = "Tests validity of transpose function.",
	.run = test,
TEST_END
