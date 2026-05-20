

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "4( 32(rv64gc_FMUL.D_64b) )";
unsigned long block_inst = 32;
unsigned long lanes = 1;
unsigned long block_ops = (32*(1*lanes));
unsigned long unroll = 4;

void set_all_reg()
{
    int avl = 1;
    
    

    double *mem = (double *)malloc(avl * sizeof(double));
    srand(time(NULL));
    for (size_t i = 0; i < avl; i++){
	    mem[i] = ( double )((float)rand() / RAND_MAX * 10.0);
    printf("init value%ld=%f\t",i, (double)mem[i]);
   }
   printf("\n");
    
    asm volatile (
        "fld f0, (%0)\n\t"
        "fld f1, (%0)\n\t"
        "fld f2, (%0)\n\t"
        "fld f3, (%0)\n\t"
        "fld f4, (%0)\n\t"
        "fld f5, (%0)\n\t"
        "fld f6, (%0)\n\t"
        "fld f7, (%0)\n\t"
        "fld f8, (%0)\n\t"
        "fld f9, (%0)\n\t"
        "fld f10, (%0)\n\t"
        "fld f11, (%0)\n\t"
        "fld f12, (%0)\n\t"
        "fld f13, (%0)\n\t"
        "fld f14, (%0)\n\t"
        "fld f15, (%0)\n\t"
        "fld f16, (%0)\n\t"
        "fld f17, (%0)\n\t"
        "fld f18, (%0)\n\t"
        "fld f19, (%0)\n\t"
        "fld f20, (%0)\n\t"
        "fld f21, (%0)\n\t"
        "fld f22, (%0)\n\t"
        "fld f23, (%0)\n\t"
        "fld f24, (%0)\n\t"
        "fld f25, (%0)\n\t"
        "fld f26, (%0)\n\t"
        "fld f27, (%0)\n\t"
        "fld f28, (%0)\n\t"
        "fld f29, (%0)\n\t"
        "fld f30, (%0)\n\t"
        "fld f31, (%0)\n\t"
    : /* no input */
    : "r"(mem)
    : "memory"
    );


free(mem);
}



void kernel(unsigned long iters)
{
    for (unsigned long i=0; i<iters; ++i) {
        asm volatile (
        "fmul.d f0, f8, f16 \n\t"
        "fmul.d f1, f9, f17 \n\t"
        "fmul.d f2, f10, f18 \n\t"
        "fmul.d f3, f11, f19 \n\t"
        "fmul.d f4, f12, f20 \n\t"
        "fmul.d f5, f13, f21 \n\t"
        "fmul.d f6, f14, f22 \n\t"
        "fmul.d f7, f15, f23 \n\t"
        "fmul.d f8, f16, f24 \n\t"
        "fmul.d f9, f17, f25 \n\t"
        "fmul.d f10, f18, f26 \n\t"
        "fmul.d f11, f19, f27 \n\t"
        "fmul.d f12, f20, f28 \n\t"
        "fmul.d f13, f21, f29 \n\t"
        "fmul.d f14, f22, f30 \n\t"
        "fmul.d f15, f23, f31 \n\t"
        "fmul.d f16, f24, f0 \n\t"
        "fmul.d f17, f25, f1 \n\t"
        "fmul.d f18, f26, f2 \n\t"
        "fmul.d f19, f27, f3 \n\t"
        "fmul.d f20, f28, f4 \n\t"
        "fmul.d f21, f29, f5 \n\t"
        "fmul.d f22, f30, f6 \n\t"
        "fmul.d f23, f31, f7 \n\t"
        "fmul.d f24, f0, f8 \n\t"
        "fmul.d f25, f1, f9 \n\t"
        "fmul.d f26, f2, f10 \n\t"
        "fmul.d f27, f3, f11 \n\t"
        "fmul.d f28, f4, f12 \n\t"
        "fmul.d f29, f5, f13 \n\t"
        "fmul.d f30, f6, f14 \n\t"
        "fmul.d f31, f7, f15 \n\t"
        "fmul.d f0, f8, f16 \n\t"
        "fmul.d f1, f9, f17 \n\t"
        "fmul.d f2, f10, f18 \n\t"
        "fmul.d f3, f11, f19 \n\t"
        "fmul.d f4, f12, f20 \n\t"
        "fmul.d f5, f13, f21 \n\t"
        "fmul.d f6, f14, f22 \n\t"
        "fmul.d f7, f15, f23 \n\t"
        "fmul.d f8, f16, f24 \n\t"
        "fmul.d f9, f17, f25 \n\t"
        "fmul.d f10, f18, f26 \n\t"
        "fmul.d f11, f19, f27 \n\t"
        "fmul.d f12, f20, f28 \n\t"
        "fmul.d f13, f21, f29 \n\t"
        "fmul.d f14, f22, f30 \n\t"
        "fmul.d f15, f23, f31 \n\t"
        "fmul.d f16, f24, f0 \n\t"
        "fmul.d f17, f25, f1 \n\t"
        "fmul.d f18, f26, f2 \n\t"
        "fmul.d f19, f27, f3 \n\t"
        "fmul.d f20, f28, f4 \n\t"
        "fmul.d f21, f29, f5 \n\t"
        "fmul.d f22, f30, f6 \n\t"
        "fmul.d f23, f31, f7 \n\t"
        "fmul.d f24, f0, f8 \n\t"
        "fmul.d f25, f1, f9 \n\t"
        "fmul.d f26, f2, f10 \n\t"
        "fmul.d f27, f3, f11 \n\t"
        "fmul.d f28, f4, f12 \n\t"
        "fmul.d f29, f5, f13 \n\t"
        "fmul.d f30, f6, f14 \n\t"
        "fmul.d f31, f7, f15 \n\t"
        "fmul.d f0, f8, f16 \n\t"
        "fmul.d f1, f9, f17 \n\t"
        "fmul.d f2, f10, f18 \n\t"
        "fmul.d f3, f11, f19 \n\t"
        "fmul.d f4, f12, f20 \n\t"
        "fmul.d f5, f13, f21 \n\t"
        "fmul.d f6, f14, f22 \n\t"
        "fmul.d f7, f15, f23 \n\t"
        "fmul.d f8, f16, f24 \n\t"
        "fmul.d f9, f17, f25 \n\t"
        "fmul.d f10, f18, f26 \n\t"
        "fmul.d f11, f19, f27 \n\t"
        "fmul.d f12, f20, f28 \n\t"
        "fmul.d f13, f21, f29 \n\t"
        "fmul.d f14, f22, f30 \n\t"
        "fmul.d f15, f23, f31 \n\t"
        "fmul.d f16, f24, f0 \n\t"
        "fmul.d f17, f25, f1 \n\t"
        "fmul.d f18, f26, f2 \n\t"
        "fmul.d f19, f27, f3 \n\t"
        "fmul.d f20, f28, f4 \n\t"
        "fmul.d f21, f29, f5 \n\t"
        "fmul.d f22, f30, f6 \n\t"
        "fmul.d f23, f31, f7 \n\t"
        "fmul.d f24, f0, f8 \n\t"
        "fmul.d f25, f1, f9 \n\t"
        "fmul.d f26, f2, f10 \n\t"
        "fmul.d f27, f3, f11 \n\t"
        "fmul.d f28, f4, f12 \n\t"
        "fmul.d f29, f5, f13 \n\t"
        "fmul.d f30, f6, f14 \n\t"
        "fmul.d f31, f7, f15 \n\t"
        "fmul.d f0, f8, f16 \n\t"
        "fmul.d f1, f9, f17 \n\t"
        "fmul.d f2, f10, f18 \n\t"
        "fmul.d f3, f11, f19 \n\t"
        "fmul.d f4, f12, f20 \n\t"
        "fmul.d f5, f13, f21 \n\t"
        "fmul.d f6, f14, f22 \n\t"
        "fmul.d f7, f15, f23 \n\t"
        "fmul.d f8, f16, f24 \n\t"
        "fmul.d f9, f17, f25 \n\t"
        "fmul.d f10, f18, f26 \n\t"
        "fmul.d f11, f19, f27 \n\t"
        "fmul.d f12, f20, f28 \n\t"
        "fmul.d f13, f21, f29 \n\t"
        "fmul.d f14, f22, f30 \n\t"
        "fmul.d f15, f23, f31 \n\t"
        "fmul.d f16, f24, f0 \n\t"
        "fmul.d f17, f25, f1 \n\t"
        "fmul.d f18, f26, f2 \n\t"
        "fmul.d f19, f27, f3 \n\t"
        "fmul.d f20, f28, f4 \n\t"
        "fmul.d f21, f29, f5 \n\t"
        "fmul.d f22, f30, f6 \n\t"
        "fmul.d f23, f31, f7 \n\t"
        "fmul.d f24, f0, f8 \n\t"
        "fmul.d f25, f1, f9 \n\t"
        "fmul.d f26, f2, f10 \n\t"
        "fmul.d f27, f3, f11 \n\t"
        "fmul.d f28, f4, f12 \n\t"
        "fmul.d f29, f5, f13 \n\t"
        "fmul.d f30, f6, f14 \n\t"
        "fmul.d f31, f7, f15 \n\t"
        : /* no input */
        : /* no output */
        : 
        );
    }
}
