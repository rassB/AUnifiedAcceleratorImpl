#include "mult.h"





/*
void fillarrays(KerType alpha[mr][k*k*c], ImgType beta[k*k*c][mc])

{
	for (int i=0;i<mr;i++)
	{
		for(int j=0;j<k*k*c;j++)
		{
			alpha [i][j]=(i==mr-1 && j== k*k*c-1)? 1:0;
		}
	}



	for (int i=0;i<k*k*c;i++)
	{
		for(int j=0;j<mc;j++)
		{

			beta [i][j]=(i==(k*k*c)-1 && j== mc-1)? 1:0;
		}

	}

}*/

void macarray(KerType A[mr][k*k*c], ImgType B[k*k*c][mc],MidType C[mr][mc])
{

	MidType last,temp;

	for(int t=0;t<k*k*c;t++)
	{
#pragma HLS PIPELINE
		for(int i=0; i<mr; i++)
		{
#pragma HLS UNROLL
			for(int j=0; j<mc; j++)
			{
#pragma HLS UNROLL
				last = (t==0)? "0b00000000000000000000000000000000":C[i][j];
				temp = A[i][t]*B[t][j];
				C[i][j]= last+temp;
			}
		}
	}
}






void synth(KerType A[mr][k*k*c],ImgType B[k*k*c][mc], MidType output[mr][mc])
{

#pragma HLS ARRAY_PARTITION variable=A type=complete
#pragma HLS ARRAY_PARTITION variable=B type=complete
#pragma HLS ARRAY_PARTITION variable=output type=complete

	//fillarrays(A, B);
macarray(A, B,output);
}


/*

// Test for the mult Operation, Unrolling gives 9 DSPs. further testing is needed (With BindOp for operators and Memory BindOp).
void synth(KerType A[mr][k*k*c],ImgType B[k*k*c][mc], MidType output[mr][mc])
{

#pragma HLS ARRAY_PARTITION variable=A type=complete
#pragma HLS ARRAY_PARTITION variable=B type=complete
#pragma HLS ARRAY_PARTITION variable=output type=complete

	//fillarrays(A, B);
macarray(A, B,output);
}
*/
