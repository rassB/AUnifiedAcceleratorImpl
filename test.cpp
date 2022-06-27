#include "mult.h"
#include <iostream>

#include <common/xf_common.hpp>
#include <common/xf_headers.hpp>
#include <common/xf_utility.hpp>
#include <opencv2/core.hpp>
#include <sstream>
#include "common/xf_sw_utils.hpp"
#include "opencv2/opencv.hpp"
#include <common/xf_structs.hpp>
#include "imgproc/xf_channel_extract.hpp"

#include <imgproc/xf_channel_combine.hpp>

//#include <bitset>

void macarray_c(KerType A[mr][c*k*k], ImgType B[c*k*k][mc],MidType C[mr][mc])
{



	//std::bitset<4> b3{"0011"};
	MidType last,temp;

	for(int t=0;t<c*k*k;t++)
	{
		for(int i=0; i<mr; i++)
		{
			for(int j=0; j<mc; j++)
			{
				last = (t==0)? MidType_ZERO:C[i][j];
				temp = A[i][t]*B[t][j];
				C[i][j]= last+temp;
				std::cout<< "Element C[" << i<<","<<j<<"]= A["<<i<<","<<t<<"]("<<A[i][t]<<")*B["<<t<<","<<j<<"]("<<B[t][j]<<")+ oldC["<<i<<","<<j<<"]"<< last<<"= "<<C[i][j]<<std::endl;
				//std::cout<< b3;
				//NOTE The outputting problem seems to be that cout doesn't consider my fixed points "correctly".
			}
		}
	}


	for (int dii=0; dii<mr; dii++)
	{
		for(int djj=0; djj<mc; djj++)
		{
			std::cout<< C[dii][djj]<<' ';
		}
		std::cout<< std::endl;
	}


}
void fillarrays_c(KerType alpha[mr][k*k*c], ImgType beta[c*k*k][mc])

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
		for (int j = 0; j < c; ++j)
		{
			for (int x = 0; x < k; ++x)
			{
				for (int y = 0; y < k; ++y)
				{
					alpha[i][j][x][y]= completed++;
				}
			}
		}
	}
	for (int ii = 0; ii < mr; ++ii)
	{
		for (int jj = 0; jj < c; ++jj)
		{
			for (int xx = 0; xx < k; ++xx)
			{
				for (int yy = 0; yy < k; ++yy)
				{
					std::cout<< alpha[ii][jj][xx][yy]<<" ";
				}
				std::cout<<std::endl;
			}
			std::cout<<std::endl;
		}
		std::cout<<std::endl;
	}

}
void kernel_mapping_c(KerType initial_kernel[mr][c][k][k],KerType flat_kernel[mr][c*k*k])
{
	//64 ×3 kernels with a size of 3 ×3 are mapped to a rearranged matrix with dimensions of 64 ×(3 ×3 ×3).  (they use 64 mc in their impl.)
	//void kernel_mapping_c(KerType initial_kernel[mr][c][k][k],KerType flat_kernel[mr][c*k*k]);
	int mr_counter =0;

	for (int i = 0; i < mr; ++i)
	{
		mr_counter =0;
		for (int j = 0; j < c; ++j)
		{
			for (int x = 0; x < k; ++x)
			{
				for (int y = 0; y < k; ++y)
				{
					flat_kernel[i][mr_counter]=initial_kernel[i][j][x][y];
					mr_counter++;
				}
			}
		}
	}

	for (int ii = 0; ii < mr; ++ii)
	{
		for (int jj = 0; jj < c*k*k; ++jj)
		{
			std::cout<<flat_kernel[ii][jj]<< " ";
		}
		std::cout<<std::endl;
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



// initial input comes as a stream, to be decided.
// input mapping takes initial input and converts it to flat input

void input_mapping_naive_c(ImgType initial_input[c][h][l], ImgType flat_input[c*k*k][h*l])

{
//TODO (Solved) out of bound junk has to be cleared, with data type options ? rounding and overflow ? VS detected and put to 0
	/* Done, Not very elegant but shifterators are too important to be touched,
	 * more logic is needed to handle the out of bound cases without accessing prohibited memory on the board.
	 */
//TODO Padding (Do I need any more padding than that ? am not convinced since my logic implments 0s where out of bound.

	for (int icol = 0; icol < h*l; ++icol)
	{
	int kernel_channel_count=0;
		for (int i = 0; i < c; ++i)
		{
			for (int j = 0; j < k; ++j)
			{
				for (int x = 0; x < k; ++x)
				{
					//(x+icol > l*h)? flat_input[kernel_channel_count][icol]= ImgType_ZERO :flat_input[kernel_channel_count][icol] = initial_input[i][j][x+icol];


					//Sanity Check ?
//TODO write this sanity check in a more elegant way; that would avoid writing if it's to overwrite just after.
					if (!(x+icol<l))
					{
						std::cout <<"non-existing input element ["
								  <<i<<","<<j<<","<< x+icol
								  <<"] value="<< initial_input[i][j][x+icol]
                                  <<" is being replaced by["
								  << i <<","<<j+1<<","<<(x+icol)-l
								  <<"] value="<< initial_input[i][j+1][(x+icol)-l]
                                  <<" is being put in flat["
								  << kernel_channel_count<<","<<icol<<']'
								  <<std::endl;
						flat_input[kernel_channel_count][icol] = initial_input[i][j+1][x+icol-l];
						if(!(j+1<h))
						{
							std::cout <<" INNER SANITY :non-existing input element ["
									  <<i<<","<<j+1<<","<< x+icol-l
									  <<"] value="<< initial_input[i][j+1][x+icol-l]
	                                  <<" is being replaced by["
									  << i+1 <<","<<j+1-h<<","<<(x+icol)-l
									  <<"] value="<< initial_input[i+1][j+1-h][(x+icol)-l]
	                                  <<" is being put in flat["
									  << kernel_channel_count<<","<<icol<<']'
									  <<std::endl;
							flat_input[kernel_channel_count][icol] = initial_input[i+1][j+1-h][x+icol-l];
							if(!(i+1<c))
							{
								std::cout <<"   INNER SANITY :non-existing input element ["
										  <<i+1<<","<<j+1-h<<","<< x+icol-l
										  <<"] value="<< initial_input[i+1][j+1-h][x+icol-l]
		                                  <<" is being replaced by["
										  << ImgType_ZERO
										  <<"] value="<< ImgType_ZERO
		                                  <<" is being put in flat["
										  << kernel_channel_count<<","<<icol<<']'
										  <<std::endl;
								flat_input[kernel_channel_count][icol] = ImgType_ZERO;
							}
						}
					}


					else {
						std::cout<<"normal"<<std::endl;

						flat_input[kernel_channel_count][icol] = initial_input[i][j][x+icol];}

					//Element Corresp Check
					//std::cout << "element Of flat input :[" << kernel_channel_count << ","<<icol << "] "<< "corresponds to element of initial input:["<< i <<","<<j<<"," <<x+icol<<"] and is equal to: "<< flat_input[kernel_channel_count][icol] <<std::endl;


					kernel_channel_count++; //return value before increment
				}
			}
		}
	}

/* Mysterious Behavior ? Explanation
 Naive Implementation of the code is technically wrong, debugging with indexes shows that it accesses memory
 that it should never have access to WITHOUT giving segmentation faults, because apparently VitisHLS has some GodMode
 I wonder what kind of security issues this could provide. or did i just discover some Memory Leak ? ANYWAY.
in the code above for h=3 l=3 c=2, the code accesses non-existing input element [0,0,4] value=4
and puts it in flat[2,4], while this should have given a segmentation fault in a sane universe, Right ?
but since the memory allocation is contiguous (i guess?), and the elements exist in the input array it stills outputs
the "Correct" value.


It also seems like either "more" memory is allocated than necessary because the code has access to some junk values after values
become junk? (magic 0s, negative elements etc.)

I don't know how this could be Maliciously exploited, or is just used as a coding paradigm.
 What is astonishing is that the outputs are always "as they should be" for correct code, chances are this is a valid coding paradigm.
More Testing should be done on the limits of this, because memory is probably allocation by the app to be = to the memory available
on the Specificed Board for CoSimulation.
If this is not the case then there is potential for OS level exploits.
Enough Internet for today
 */



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




void altinput_mapping_naive_c(ImgType initial_input[c][h][l], ImgType flat_input[c*h*k][l])
{

	/*ToDO: Add Padding, size of flat input should be flatinput[c*k*k][mc+2pad], this flat input is going to be used as input for macarray
	 * Not working correctly for h l 28 28, because for (int x = 0; x < mc; ++x) is limitng to MC elements ?
	 * Not fetching more lines Yet. (fetching lines is done at top level).
	 */
	int count=0;

		for (int i = 0; i < c; ++i)
			{
				for (int j = 0; j < h; ++j)
					{
							for (int shifter = 0; shifter < k; ++shifter) //shift more than k times ?
								{
									for (int x = 0; x < l; ++x)
										{
										(!(x+shifter<l))?flat_input[count][x]=ImgType_ZERO:flat_input[count][x]=initial_input[i][j][x+shifter];

									//	std::cout<<"Initial:["<<i<<","<<j<<","<<x+shifter<<"]Value:"<<initial_input[i][j][x+shifter]<<" Goes to Flat:["<<count<<","<<x<<"]Value:"<<flat_input[count][x]<<std::endl;

										}
									count++;
								}
					}
			}






	//print flattened array
		for (int ii = 0; ii < c*h*k; ++ii)
		{
			for (int jj = 0; jj < l; ++jj)
			{
				std::cout << flat_input[ii][jj] << " ";
			}
			std::cout << std::endl;
		}


}





void partial_matrix_mapping(ImgType initial_input[c][h][l], ImgType flat_input[c*k*k][mc+2*pad])
	{

	/*ToDO: Add Padding, size of flat input should be flatinput[c*k*k][mc+2pad], this flat input is going to be used as input for macarray
	 //SOLVED : Working Fine, always has been, limiting to MC elements is by design. Not working correctly for h l 28 28, because for (int x = 0; x < mc; ++x) is limitng to MC elements ?
	 * Not fetching more lines Yet. (fetching lines is done at top level).
	 */

	int count=0;

		for (int i = 0; i < c; ++i)
			{
				for (int j = 0; j < k; ++j)
					{
							for (int shifter = 0; shifter < k; ++shifter) //shift more than k times ?
								{
									for (int x = 0; x < mc; ++x)
										{
										(!(x+shifter<mc))?flat_input[count][x]=ImgType_ZERO:flat_input[count][x]=initial_input[i][j][x+shifter];



										}
									count++;
								}
					}
			}


		//print flattened array
				for (int ii = 0; ii < c*k*k; ++ii)
				{
					for (int jj = 0; jj < mc+2*pad; ++jj)
					{
						std::cout << flat_input[ii][jj] << " ";
					}
					std::cout << std::endl;
				}



	}


void conv_layer(ImgType ibuf[c*k*k][mc+2*pad], KerType kbuf[mr][c*k*k],MidType obuf[mr][mc])
		{

	// Load Kernels

	// Acquire Image element
	// for a block
	// map block
	// convolve block
	// store block



		}



/*
void padding(ImgType initial_input[c][h][l])
{

for (int i = 0; i < height; i++)
	{
        for (int j = 0; j < width; j++)


            if (i == 0 || (i > r_width && i < height - border_width))
            {
                // read a pixel out of the input stream and cache it for
                // immediate use and later replication purposes
                if (j < width - (K - 1))
                {
                    pix_in = vconv.read();
                    borderbuf[j] = pix_in;
                }
                if (j == 0) {
                    l_edge_pix = pix_in;
                }
                if (j == width - K)
                {
                    r_edge_pix = pix_in;
                }
            }
            // Select output value from the appropriate cache resource
            if (j <= border_width)
            {
                pix_out = l_edge_pix;
            }
            else if (j >= width - border_width - 1)
            {
                pix_out = r_edge_pix;
            }
            else
            {
                pix_out = borderbuf[j - border_width];
            }
        }
	}
*/


int main()
{
/*test macarray
	KerType A[mr][k*k*c];
	ImgType B[k*k*c][mc];

fillarrays_c(A,B);
macarray_c(A,B);*/

/* test kernel mapping

	KerType A[mr][c][k][k];
	KerType B[mr][c*k*k];
	fillkernels_c(A);
	kernel_mapping_c(A, B); */

/* test kernel mapping

ImgType A[c][h][l];
ImgType B[c*k*k][h*l];

fillinputs_c(A);

input_mapping_naive_c(A,B);

ImgType A[c][h][l];
ImgType B[c*h*k][l];

fillinputs_c(A);
altinput_mapping_naive_c(A, B);
*/
/*
ImgType A[c][h][l], B[c*k*k][mc+2*pad];
KerType alpha[mr][c][k][k], ker[mr][k*k*c];
MidType out[mr][mc];

	fillinputs_c(A);
	partial_matrix_mapping(A,B); */


	/*


fillkernels_c(alpha);
kernel_mapping_c(alpha, ker);
macarray_c(ker,B,out);
*/

/*
Channel Splitting Unecessary ?

	//Simulate Data Existing from PS.
	cv::Mat  img = cv::imread("/home/rass/AUnifiedAcceleratorImpl/dataset/50x50.bmp",1);
	cv::Mat ch[img.channels()];

	for (int i = 0; i < img.channels(); ++i)
	{
		cv::extractChannel(img, ch[i], i);
	}


	xf::cv::Mat<XF_8UC3,50,50,XF_NPPC1> array;
	xf::cv::Mat<XF_8UC1,50,50,XF_NPPC1>channels[array.channels()];
	array.copyTo(img.data);

	for (int chh = 0; chh < array.channels(); ++chh)
	{
		xf::cv::extractChannel(array, channels[chh], chh);
	}


for (int i = 0; i < array.channels(); ++i)
{
	for (int j = 0; j < 50; ++j)
	{
		for (int x = 0; x < 50; ++x)
		{
			std::cout << uint8_t(ImgType(channels[i].read(x+j*50))) << " ";  //casted into uint for display
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;


}


/*
		for (int j = 0; j < h; ++j)
		{
			for (int x = 0; x < l; ++x)
			{

			std::cout <<  ImgType(array.read(x+j*l)) << " ";

			}
			std::cout << std::endl;
		}*/




//Simulate Data Existing from Software.
	cv::Mat  img = cv::imread("/home/rass/AUnifiedAcceleratorImpl/dataset/50x50.bmp",1);

	xf::cv::Mat<XF_8UC3,h,l,XF_NPPC1> array;   //8Bits, 3Channels, 50x50, Number of Pixels per clock cycle 1

	array.copyTo(img.data);

	xf::cv::Mat<XF_8UC1,50,50,XF_NPPC1>channels[array.channels()];

// to get the CHW order (4.2 paper), turns out channel splitting seems necessary.

	for (int chh = 0; chh < array.channels(); ++chh)
	{
		xf::cv::extractChannel(array, channels[chh], chh);
	}

// write to an array to test the MAC.

	ap_uint<8> A[c][h*l]; // internal array

for (int i = 0; i < c; ++i)
{
	xf::cv::xfMat2Array(channels[i], A[i]);

}




//visualisation :
/*
	for (int i = 0; i < c; ++i)
		{
		for (int j = 0; j < h; ++j)
			{
			for (int x = 0; x < l; ++x)
				{
				std::cout <<  ImgType(uint8_t (A[i][x+j*l])) << " ";
				}
			std::cout<< std::endl;
			}
		std::cout << std::endl;
		}*/

/*ImgType B[c][h][l];

for (int i = 0; i < c; ++i)
	{
	for (int j = 0; j < h; ++j)
		{
		for (int x = 0; x < l; ++x)
			{
			B[i][j][x] = A[i][x+j*l];
			}
		}
	}*/

/*
for (int i = 0; i < c; ++i)
	{
	for (int j = 0; j < h; ++j)
		{
		for (int x = 0; x < l; ++x)
			{
			std::cout <<  B[i][j][x] <<" ";
			}
		std::cout<< std::endl;
		}
	std::cout << std::endl;
	} */
	ImgType B[c][h][l];
	fillinputs_c(B);

	xf::cv::Window<c*k,l,ImgType> featureBuffer[k];


//Read K rows of each channel and put them in a buffer. No tiling implemented Yet.
	for (int chan = 0; chan < c; ++chan)
	{
		for (int Wrow = 0; Wrow < k; ++Wrow)
		{
			for (int cpy = 0; cpy < k; ++cpy)
			{
				featureBuffer[cpy].insert_row( &B[chan][Wrow][0], Wrow+chan*c); //B will be streamed, not implemented yet.
			}
		}
	}

for (int shift = 0; shift < k; ++shift) //replicates pixels on shifting automatically, using this is much better than using static
										//Static tables used to read data from other dimensions on overflow.
	{
		for (int var = 0; var < k-shift; ++var)
			{
				featureBuffer[k-shift].shift_pixels_left();
			}
	}

std::cout << std::endl << "----------------------------Output Feature ----------------------------------" << std::endl;
featureBuffer[0].window_print();
featureBuffer[1].window_print();
featureBuffer[2].window_print();



return 0;

}



