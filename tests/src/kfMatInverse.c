#include "test.h"
#include "kfMath.h"

static float rf(){ return ((random() % 1024) / 512.0f) - 1.0f; }

static int test(void)
{
	float N_temp[2][2] = {
		{ 4, 3 },
		{ 3, 2 },
	};

	float N_inv_temp[2][2] = {
		{ -2,  3 },
		{  3, -4 },
	};

	kfMat_t N = kfMatWithCols((float*)N_temp, 2);
	kfMat_t N_T = kfMatWithCols((float*)N_temp, 2);
	kfMat_t N_inv = kfMatWithCols((float*)N_inv_temp, 2);

	kfMatPrint(N, 2);
	Log(" N^-1 ", 0);
	kfMatPrint(N_inv, 2);
	Log("mat size %d, inverting...", 1, sizeof(N)); 
	kfMat2Inverse(N_inv, N, N_T);
	kfMatPrint(N_inv, 2);

	for(int i = 2; i--;){
		for(int j = 2; j--;){
			if(N_inv[i][j] != N_inv_temp[i][j]) return -1;
		}
	}

	float M_temp[3][3] = {
		{ 1, 0, 5 },
		{ 2, 1, 6 },
		{ 3, 4, 0 },
	};

	float M_inv_temp[3][3] = {
		{ -24,  20, -5 },
		{  18, -15,  4 },
		{   5,  -4,  1 },
	};

	kfMat_t I = kfMatWithCols((float*)M_temp, 3);
	kfMat_t M = kfMatWithCols((float*)M_temp, 3);
	kfMat_t M_T = kfMatWithCols((float*)M_temp, 3);
	kfMat_t M_inv = kfMatWithCols((float*)M_inv_temp, 3);

	kfMatPrint(M, 3);
	Log(" M^-1 ", 0);
	kfMatPrint(M_inv, 3);
	Log("mat size %d, inverting...", 1, sizeof(M)); 
	kfMat3Inverse(M_inv, M, M_T);
	kfMatPrint(M_inv, 3);

	for(int i = 3; i--;){
		for(int j = 3; j--;){
			if(M_inv[i][j] != M_inv_temp[i][j]) return -2;
		}
	}

	Log("Testing multiplication and correctness of 3x3 inversion", 1);
	kfMatMul(I, M, M_inv, 3);
	Log("Mul result", 1);
	kfMatPrint(I, 3);

	// assert that I is indeed the identitity
	for(int i = 3; i--;){
		for(int j = 3; j--;){
			if(i == j){
				if(I[i][j] != 1) return -3;
			}
			else if(I[i][j] != 0) return -4;
		}
	}

	return 0;
}

TEST_BEGIN
	.name = "kfMatInverse()",
	.description = "Tests validity of inversion functions.",
	.run = test,
TEST_END
