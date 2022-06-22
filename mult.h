#ifndef MULT_H
#define MULT_H

//#include <assert.h>
#include <stdint.h>
//#include <hls_stream.h>
#include <ap_fixed.h>

// Systolic Array Sizing :
#define mc 5 //columns of the Systolic Array
#define mr 3 //rows of the Systolic Array
/*TODO (SOLVED) Impl doesn't return correct results when mr =/= mc ?? WHY ? (Reasons and alternative solutions in comment)
//Solutions: index by index output seems to return expected MAC operations;
//Why output bad : In general, the output of std::cout is neither relevant nor a good VALUE debugging strategy,
	//there seems to be a problem with COUT expecting specific types (obviously it does not use fixed point by default),
	//if you still want to debug that way you should :
		//use fixed point cout (that exists idk if the implementation of FxPoints in CPP would support Xilinx Formats),
		//Can try read the indexes of the ouput matrixes this way:
			//Array[height][width][depth][indexOfElementIwantToSee] to get the bit by bit output.

	//OtherDebugging Strategies :
		//some more explicit function that just outputs by IO buffer without touching anything.
		//i should try sending in actual data (images?) and check the ouput.
//Conclusion : mc and mr can be "arbitrary". */

// Kernel Sizing :
#define k 3 //Square kernel size (must be odd?)


// Input Sizing :
#define h 28 // height of the input // temporary making 10x10
#define l 28 // length of the input
#define c 2 // channels of the input

// Padding Strategy
#define pad 0

// TODO:to be defined with primitives only.
#define kdepth 9 // number of elements inside each 3D kernel ?
#define idepth c*k*k //
#define odepth //




// Data Types :
typedef ap_fixed<8,8> KerType; // 1 bit entier et 7 de virgule :) ==> will give you negative overflow // temporary making 8,8 for tests
typedef ap_fixed<16,16> ImgType; // 8 entiers 8 vigule // temporary making it 16 16 for tests
typedef ap_fixed<17,17> MidType; // 17 entiers 15 virgule :)
// Zeros
#define  ImgType_ZERO "0b000000000"
// AND DONT CAST ANYTHING JUST LEARN TO COUNT!

/*
typedef bool KerType;
typedef int16_t ImgType;
typedef int32_t MidType;
*/


//Impl
void macarray(KerType A[mr][k*k*c],ImgType B[k*k*c][mc]);
void fillarrays(KerType alpha[mr][k*k*c], ImgType beta[k*k*c][mc]);
void matrix_mapping(ImgType[c][h][l],ImgType[c*k*k][mc+2*pad]);



//testing SysArray Arrays are already GMEE format.
void fillarrays_c(KerType alpha[mr][k*k*c], ImgType beta[k*k*c][mc]);
void macarray_c(KerType A[mr][k*k*c],ImgType B[k*k*c][mc]); // will MACarray and COUT


//Testing Mapping :
void fillkernels_c(KerType alpha[mr][c][k][k]);
void fillinputs_c(ImgType initial_input[c][h][l]);


void kernel_mapping_c(KerType initial_kernel[mr][c][k][k],KerType flat_kernel[mr][c*k*k]); //64 ×3 kernels with a size of 3 ×3 are mapped to a rearranged matrix with dimensions of 64 ×(3 ×3 ×3).  they use 64 mc in their impl.
void input_mapping_naive_c(ImgType initial_input[c][h][l], ImgType flat_input[c*k*k][h*l]); //all three input features with dimensions of 32 ×32 are mapped to a rearranged matrix with dimensions of (3 ×3 ×3) ×(32 ×32).


void altinput_mapping_naive_c(ImgType initial_input[c][h][l], ImgType flat_input[c*h*k][l]);



// to do output ??


#endif
