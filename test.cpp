#include "mult.h"
#include <iostream>
//#include <bitset>

void macarray_c(KerType A[mr][k*k*c], ImgType B[k*k*c][mc])
{

	std::bitset<4> b3{"0011"};
	MidType C[mr][mc],last,temp;

	for(int t=0;t<k*k*c;t++)
	{
		for(int i=0; i<mr; i++)
		{
			for(int j=0; j<mc; j++)
			{
				last = (t==0)? "0b00000000000000000000000000000000":C[i][j];
				temp = A[i][t]*B[t][j];
				C[i][j]= last+temp;
				std::cout<< "Element C[" << i<<","<<j<<"]= A["<<i<<","<<t<<"]("<<A[i,t]<<")*B["<<t<<","<<j<<"]("<<B[t,j]<<")+ oldC["<<i<<","<<j<<"]= "<<C[i,j]<<std::endl;
				//std::cout<< b3;
			}
		}
	}


	for (int i=0;i<mc;i++)
	{
		for(int j=0;j<mr;j++)
		{
			std::cout<< C[i][j]<<' ';
		}
		std::cout<< std::endl;
	}


}
void fillarrays_c(KerType alpha[mr][k*k*c], ImgType beta[k*k*c][mc])

{
	for (int i=0;i<mr;i++)
	{
		for(int j=0;j<k*k*c;j++)
		{

			alpha [i][j]=(i==mr-1 && j== k*k*c-1)? 0b000000000000001:0b000000000000000;
			std::cout<< alpha[i][j]<<' ';
		}
		std::cout <<std::endl;
	}



	for (int i=0;i<k*k*c;i++)
	{
		for(int j=0;j<mc;j++)
		{

			beta [i][j]=(i==(k*k*c)-1 && j== mc-1)? 1:0;
			std::cout<< beta[i][j]<<' ';
		}
		std::cout<< std::endl;
	}

	std::cout <<std::endl;

}
void fillkernels_c(KerType alpha[mr][c][k][k])
{
	int completed=0;

	for (int i = 0; i < mr; ++i)
	{
		std::cout<<"Index of Kernel mr: "<< i<< std::endl;
		for (int j = 0; j < c; ++j)
		{
			std::cout<<"Channel c: "<<j <<" of kernel mr: "<< i<<std::endl;
			for (int x = 0; x < k; ++x)
			{
				for (int y = 0; y < k; ++y)
				{
					alpha[i][j][x][y]= completed++;
					std::cout<< alpha[i][j][x][y]<<' ';
				}
				std::cout<< std::endl;
			}
			std::cout<< std::endl;
		}
		std::cout<< std::endl;
	}
}
void kernel_mapping_c(KerType initial_kernel[mr][c][k][k],KerType flat_kernel[mr][c*k*k])
{
	//64 ×3 kernels with a size of 3 ×3 are mapped to a rearranged matrix with dimensions of 64 ×(3 ×3 ×3).  (they use 64 mc in their impl.)
	//void kernel_mapping_c(KerType initial_kernel[mr][c][k][k],KerType flat_kernel[mr][c*k*k]);
	int counter=0;

	for (int i = 0; i < mr; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			for (int x = 0; x < k; ++x)
			{
				for (int y = 0; y < k; ++y)
				{
					counter++; //return value before increment
					flat_kernel[i][counter]=initial_kernel[i][j][x][y];
					std::cout<< flat_kernel[i][counter]<<' ';
				}
			}
		}
		std::cout<< std::endl;
	}

}
void fillinputs_c(ImgType initial_input[c][h][l])
{

	int completed=0;

	for (int i = 0; i < c; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			for (int x = 0; x < l; ++x)
			{
				initial_input[i][j][x]= completed++;
				std::cout<< initial_input[i][j][x]<<' ';
			}
			std::cout<< std::endl;
		}
		std::cout<< std::endl;
	}
}





void input_mapping_c(ImgType initial_input[c][h][l], ImgType flat_input[c*k*k][h*l])
{
//TODO out of bound junk has to be cleared, with data type options ? rounding and overflow ? VS detected and put to 0
//TODO Padding


	for (int icol = 0; icol < 3; ++icol)
	{
	int kernel_channel_count=0;
		for (int i = 0; i < c; ++i)
		{
			for (int j = 0; j < k; ++j)
			{
				for (int x = 0; x < k; ++x)
				{
					flat_input[kernel_channel_count][icol] = initial_input[i][j][x+icol];

					std::cout << "element Of flat input :[" << kernel_channel_count << ","<<icol << "] "<< "corresponds to element of initial input:["<< i <<","<<j<<"," <<x+icol<<"] and is equal to: "<< flat_input[kernel_channel_count][icol] <<std::endl;


					kernel_channel_count++; //return value before increment
				}
			}
		}
	}



//print flattened array
	for (int ii = 0; ii < c*k*k; ++ii)
	{
		for (int jj = 0; jj < h*l; ++jj)
		{
			std::cout << flat_input[ii][jj] << " ";
		}
		std::cout << std::endl;
	}


}



int main()
{
/*test macarray*/
	KerType A[mr][k*k*c];
	ImgType B[k*k*c][mc];

fillarrays_c(A,B);
macarray_c(A,B);

/* test kernel mapping

	KerType A[mr][c][k][k];
	KerType B[mr][c*k*k];
	fillkernels_c(A);
	kernel_mapping_c(A, B); */

/* test kernel mapping

ImgType A[c][h][l];
ImgType B[c*k*k][h*l];
fillinputs_c(A);
input_mapping_c(A,B); */


return 0;
}



