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
	float s = rf();
	float cpy_temp[3][3] = {
		{ M_temp[0][0] * s, M_temp[0][1] * s, M_temp[0][2] * s},
		{ M_temp[1][0] * s, M_temp[1][1] * s, M_temp[1][2] * s},
		{ M_temp[2][0] * s, M_temp[2][1] * s, M_temp[2][2] * s},
	};

	kfMat_t M   = kfMatWithCols((float*)M_temp, 3);
	kfMat_t cpy = kfMatWithCols((float*)cpy_temp, 3);

	// kfMatCpy()
	
	kfMatPrint(M, 3);
	Log(" * %f = ", 0, s);
	kfMatPrint(cpy, 3);

	Log("mat size %d, scaling...", 1, sizeof(M)); 
	kfMatScl(M, M, s, 3);
	Log("Scaled", 1, NULL);
	kfMatPrint(M, 3);

	for(int i = 3; i--;){
		for(int j = 3; j--;){
			float truth = cpy[i][j];
			if(truth != M[i][j]){
				assert(M[i][j] == truth);
			}
		}
	}

	return 0;
}

TEST_BEGIN
	.name = "kfMatScl()",
	.description = "Tests validity of per-element matrix scaling function.",
	.run = test,
TEST_END
