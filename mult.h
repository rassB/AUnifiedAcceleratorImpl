#ifndef MULT_H
#define MULT_H

//#include <assert.h>
#include <stdint.h>
//#include <hls_stream.h>
#include <ap_fixed.h>


#define mc 3 //columns of the Systolic Array
#define mr 5 //rows of the Systolic Array               ALSO number of rows = number of channels ??
 // Impl doesn't return correct results when mr =/= mc ?? WHY ?
#define k 3 //Square kernel size (must be odd?)



#define h 5 // height of the input // temporary making 10x10
#define l 5 // length of the input
#define c 2 // channels of the input


// TODO:to be defined with primitives only.
#define kdepth 9 // number of elements inside each 3D kernel ?
#define idepth h*l //
#define odepth //





typedef ap_fixed<8,8> KerType; // 1 bit entier et 7 de virgule :) ==> will give you negative overflow // temporary making 8,8 for tests
typedef ap_fixed<8,8> ImgType; // 8 entiers 8 vigule // temporary making it 16 16 for tests
typedef ap_fixed<17,17> MidType; // 17 entiers 15 virgule :)

// AND DONT CAST ANYTHING JUST LEARN TO COUNT!

/*
typedef bool KerType;
typedef int16_t ImgType;
typedef int32_t MidType;
*/


//Impl
void macarray(KerType A[mr][k*k*c],ImgType B[k*k*c][mc]);
void fillarrays(KerType alpha[mr][k*k*c], ImgType beta[k*k*c][mc]);
void mapping();



//testing SysArray Arrays are already GMEE format.
void fillarrays_c(KerType alpha[mr][k*k*c], ImgType beta[k*k*c][mc]);
void macarray_c(KerType A[mr][k*k*c],ImgType B[k*k*c][mc]); // will MACarray and COUT


//Testing Mapping :
void fillkernels_c(KerType alpha[mr][c][k][k]);
void fillinputs_c(ImgType initial_input[c][h][l]);


void kernel_mapping_c(KerType initial_kernel[mr][c][k][k],KerType flat_kernel[mr][c*k*k]); //64 ×3 kernels with a size of 3 ×3 are mapped to a rearranged matrix with dimensions of 64 ×(3 ×3 ×3).  they use 64 mc in their impl.
void input_mapping_c(ImgType initial_input[c][h][l], ImgType flat_input[c*k*k][h*l]); //all three input features with dimensions of 32 ×32 are mapped to a rearranged matrix with dimensions of (3 ×3 ×3) ×(32 ×32).

// to do output ??


#endif
